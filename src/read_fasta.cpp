#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <sstream>

#include <seqan/sequence.h>
#include <seqan/file.h>

#include "read_fasta.h"

#define READ_LEN 64

using namespace seqan;
using namespace std;

/************************************************/
/* Convert a DNA sequnece on alphabet {A,C,G,T} */
/* into a binary sequence                       */
/************************************************/
unsigned long long fingerprint(const string& seq){
    unsigned long long number = 0;
    for(unsigned int i=0; i<seq.length(); i++){
        number = number<<2;
        if(seq.at(i) == 'N' || seq.at(i) == 'n'){
            number |= 0;
        }
        if(seq.at(i) == 'A' || seq.at(i) == 'a'){
            number |= 0;
        }
        if(seq.at(i) == 'C' || seq.at(i) == 'c'){
            number |= 1;
        }
        if(seq.at(i) == 'G' || seq.at(i) == 'g'){
            number |= 2;
        }
        if(seq.at(i) == 'T' || seq.at(i) == 't'){
            number |= 3;
        }
    }//End_For
    return number;
}//End_Method

/*********************************************/
/* Parse Fasta Information                   */
/*********************************************/
table_entry* parse_fasta(String<Dna5> seq, string meta){
    table_entry* el = NULL;
    string left_seq;
    assign(left_seq,prefix(seq,length(seq)/2));
    string right_seq;
    assign(right_seq,suffix(seq,length(seq)/2));
    int source_pos = meta.find("/source=");
    int gene_id_pos = meta.find("/gene_id=");
    int gene_strand_pos = meta.find("/gene_strand=");

    if(source_pos != -1 && gene_id_pos != -1 && gene_strand_pos != -1){
        source_pos += 8;
        gene_id_pos += 9;
        gene_strand_pos += 13;
        int next_stop = meta.find_first_of(" ",source_pos);
        string source = meta.substr(source_pos,next_stop - source_pos);

        next_stop = meta.find_first_of(" ",gene_id_pos);
        string gene_id = meta.substr(gene_id_pos,next_stop - gene_id_pos);

        next_stop = meta.find_first_of(" ",gene_strand_pos);
        int gene_strand;
        ::std::istringstream(meta.substr(gene_strand_pos,next_stop - gene_strand_pos)) >> gene_strand;

        int gb_pos = meta.find("/gb=", 0);
        int clone_end_pos = meta.find("/clone_end=", 0);

        if(gb_pos != -1 && clone_end_pos != -1){
            gb_pos += 4;
            clone_end_pos += 11;
            next_stop = meta.find_first_of("_",gb_pos);
            int transcript_id;
            ::std::istringstream(meta.substr(gb_pos,next_stop - gb_pos)) >> transcript_id;

            gb_pos = next_stop+1;
            next_stop = meta.find_first_of(" ",gb_pos);
            long offset;
            ::std::istringstream(meta.substr(gb_pos,next_stop - gb_pos)) >> offset;


            next_stop = meta.find_first_of(" ",clone_end_pos);
            int clone_end;
            ::std::istringstream(meta.substr(clone_end_pos,next_stop - 1 - clone_end_pos)) >> clone_end;

            el = new table_entry(seq, source, gene_id, gene_strand, transcript_id, offset, clone_end,
                                 fingerprint(left_seq), fingerprint(right_seq));
        }else{
            el = new table_entry(seq, source, gene_id, gene_strand, fingerprint(left_seq), fingerprint(right_seq));
        }//End_If
    }else{
        el = new table_entry(seq, "region", "id", 0, fingerprint(left_seq), fingerprint(right_seq));
    }//End_If
    return el;
}//End_Method

/***************************************/
/* Add entries in the "Hash Table"     */
/***************************************/
void add_entry(tables &t, table_entry* entry){
    hash_map::iterator it;
    it = t.left_map.find(entry->get_left_fingerprint());
    if(it == t.left_map.end()){
        element_table el;
        el.unspliced = 1;
        el.half_spliced = 0;
        el.p = entry;
        t.left_map[entry->get_left_fingerprint()] = el;
    }else{
        table_entry* temp = t.left_map[entry->get_left_fingerprint()].p;
        table_entry* prev;
        bool found = 0;
        do{
            if(temp->get_right_fingerprint() == entry->get_right_fingerprint()){
                found = 1;
            }
            prev = temp;
            temp = temp->get_l_next();
        }while(temp != NULL && !found);
        if(found){
            prev->increase_freq();
            delete entry;
            return;
        }else{
            t.left_map[entry->get_left_fingerprint()].unspliced = 0;
            prev->set_l_next(entry);
            entry->set_l_prev(prev);
        }//End_If
    }//End_If
    
    it = t.right_map.find(entry->get_right_fingerprint());
    if(it == t.right_map.end()){
        element_table el;
        el.unspliced = 1;
        el.half_spliced = 0;
        el.p = entry;
        t.right_map[entry->get_right_fingerprint()] = el;
    }else{
        table_entry* temp = t.right_map[entry->get_right_fingerprint()].p;
        table_entry* prev;
        do{
            prev = temp;
            temp = temp->get_r_next();
        }while(temp != NULL);
        t.right_map[entry->get_right_fingerprint()].unspliced = 0;
        prev->set_r_next(entry);
        entry->set_r_prev(prev);
    }//End_If
}//End_Method

/******************************/
/* Read a fasta file          */
/******************************/
int read_fasta(char* file_name, tables &t){
    //Struct with hash tables
    string name = file_name;
    string extension = name.substr(name.find_last_of(".") + 1);
    if(extension == "fa" || extension == "fas" || extension == "fasta"){
        unsigned long long file_dimension = 1;
	ifstream f(file_name,ios::in|ios::binary|ios::ate);
	if(f.is_open()){
            file_dimension = f.tellg();
            f.close();
	}
	else{
            ::std::cerr << "Unable to open file " << file_name << ::std::endl;
            return 1;	
	}
        unsigned long long read_size = 0;
	int perc = 0;
        ::std::fstream fstrm;
        fstrm.open(file_name, ::std::ios_base::in | ::std::ios_base::binary);
        if(fstrm.is_open()){
            ::std::cerr << "Processing RNA-seq file..." << ::std::endl;
            String<char> fasta_tag;
            String<Dna5> fasta_seq;
            int c = 0;
            while(!fstrm.eof()){
                c++;
                //::std::cerr << c << ::std::endl;
                readMeta(fstrm, fasta_tag, Fasta());
                read(fstrm, fasta_seq, Fasta());
                read_size+=(length(fasta_tag)*sizeof(char)+length(fasta_seq)*sizeof(char));
                if ((read_size/(double)file_dimension)*100 - 1 >= perc) {
                    perc++;
                    if(perc%10 == 0){
                        ::std::cerr << "Processed: " << perc << "%" << endl;
                    }
                }
                //Parse RNA-seq Sequence
                if(length(fasta_seq) >= READ_LEN){
                    for(unsigned int i = 0;i<=length(fasta_seq)-READ_LEN;i++){
                        table_entry* tab = parse_fasta(infix(fasta_seq,i,i+READ_LEN),toCString(fasta_tag));
                        add_entry(t, tab);
                    }
                }else{
                    ::std::cerr << "Invalid fasta entry" << ::std::endl;
                }
                //::std::cerr << tab->get_short_read()->get_RNA_seq_sequence() << ::std::endl;
            }//End_while
            ::std::cerr << "Processing RNA-seq file...done!" << ::std::endl;
            fstrm.close();
            
        }else{
            ::std::cerr << "Unable to open file " << file_name << ::std::endl;
            return 1;
        }//End_if
    }else{
	::std::cerr << "Error opening RNA-seq file " << file_name << ::std::endl;
        ::std::cerr << "Not a fasta file (.fa, .fas or .fasta)" << ::std::endl;
        return 2;
    }//End_if
    return 0;
}//End_Method
