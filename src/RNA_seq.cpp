/**
* RNA-seq-Graph
* Method for reconstructing the Isoform Graph of a gene from RNA-seq data
*
* Copyright (C) 2011 Stefano Beretta <ste.beretta(-at-)gmail.com>
*
* Distributed under the terms of the GNU Affero General Public License (AGPL)
*
*
* This file is part of RNA-seq-Graph.
*
* RNA-seq-Graph is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* RNA-seq-Graph is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with RNA-seq-Graph.  If not, see <http://www.gnu.org/licenses/>.
**/
/********************************/
/* Class the models the RNA-seq */
/* sequence                     */
/********************************/

#include <string>
#include <seqan/sequence.h>

#include "RNA_seq.h"

//Constructors
RNA_seq::RNA_seq(const String<Dna5>& seq){
    assign(sequence,seq);
    //No source and gene_id
    //this->source = "NONE";
    //this->gene_id = "NONE";
    //No strand
    //this->gene_strand = 0;
    //No transcript
    //this->transcript_id = -1;
    //No offset
    //this->offset = -1;
    //No clone_end
    //this->clone_end = 0;
}
/*
RNA_seq::RNA_seq(String<Dna5> seq, string source, string gene_id){
    //RNA_seq::RNA_seq(seq);
    assign(sequence,seq);
    this->source = source;
    this->gene_id = gene_id;
    //No strand
    this->gene_strand = 0;
    //No transcript
    this->transcript_id = -1;
    //No offset
    this->offset = -1;
    //No clone_end
    this->clone_end = 0;
}*/
/*
RNA_seq::RNA_seq(String<Dna5> seq, string source, string gene_id, int gene_strand){
    //RNA_seq::RNA_seq(seq);
    assign(sequence,seq);
    this->source = source;
    this->gene_id = gene_id;
    this->gene_strand = gene_strand;
    //No transcript
    this->transcript_id = -1;
    //No offset
    this->offset = -1;
    //No clone_end
    this->clone_end = 0;
}*/
/*
RNA_seq::RNA_seq(String<Dna5> seq, string source, string gene_id, int gene_strand, int transcript_id, long offset, int clone_end){
    //RNA_seq(seq, source, gene_id);
    assign(sequence,seq);
    this->source = source;
    this->gene_id = gene_id;
    this->gene_strand = gene_strand;
    this->transcript_id = transcript_id;
    this->offset = offset;
    this->clone_end = clone_end;
}*/

//Copy Constructor
RNA_seq::RNA_seq(const RNA_seq& rhs){
    assign(sequence,rhs.sequence);
    //source = rhs.source;
    //gene_id = rhs.gene_id;
    //gene_strand = rhs.gene_strand;
    //transcript_id = rhs.transcript_id;
    //offset = rhs.offset;
    //clone_end = rhs.clone_end;
}

//Overloading =
RNA_seq& RNA_seq::operator=(const RNA_seq& rhs){
    if(&rhs == this)
        return *this;
    assign(sequence,rhs.sequence);
    //source = rhs.source;
    //gene_id = rhs.gene_id;
    //gene_strand = rhs.gene_strand;
    //transcript_id = rhs.transcript_id;
    //offset = rhs.offset;
    //clone_end = rhs.clone_end;
    return *this;
}

//Set Methods
/*
void RNA_seq::set_RNA_seq_source(string source){
    this->source = source;
}

void RNA_seq::set_RNA_seq_gene_id(string gene_id){
    this->gene_id = gene_id;
}

void RNA_seq::set_RNA_seq_gene_strand(int gene_strand){
    this->gene_strand = gene_strand;
}

void RNA_seq::set_RNA_seq_transcript_id(int transcript_id){
    this->transcript_id = transcript_id;
}
void RNA_seq::set_RNA_seq_offset(long offset){
    this->offset = offset;
}

void RNA_seq::set_RNA_seq_clone_end(int){
    this->clone_end = clone_end;
}*/

//Get Mehtods
/*
string RNA_seq::get_RNA_seq_source() const{
    return source;
}

string RNA_seq::get_RNA_seq_gene_id() const{
    return gene_id;
}

int RNA_seq::get_RNA_seq_gene_strand() const{
    return gene_strand;
}

int RNA_seq::get_RNA_seq_transcript_id() const{
    return transcript_id;
}

long RNA_seq::get_RNA_seq_offset() const{
    return offset;
}

int RNA_seq::get_RNA_seq_clone_end() const{
    return clone_end;
}*/
