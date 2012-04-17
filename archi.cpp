/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>


using namespace std;

*/
#include "structureArchi.h"
/// fonctions utiles
CPU ADD_CPU(CPU mon_cpu, string name,uint freq)
{
  cpu *newcpu = new cpu;
  int nb_ligne=0;
  string line, tmp;
  ifstream cpu_file(name.c_str());
  /// on assigne les parametres du nouveau cpu
  if(cpu_file)
  {
    newcpu->name_cpu=name;
     while(cpu_file>>line ){ /// on lit mot par mot le fichier et on instantie les composants
      nb_ligne++;
      cpu_file>>tmp;
       if(line=="start_frequency:") newcpu->start_frequency =atoi(tmp.c_str());
       else{
	  if(line=="end_frequency:") newcpu->end_frequency =atoi(tmp.c_str());
	  else{
	    if(line=="dmips:") newcpu->dmips =atof(tmp.c_str());
	    else{
	      if(line=="pipeline:") newcpu->pipeline =atoi(tmp.c_str());
	      else{
		    nb_ligne--;
		    cout <<" \n\n ATTENTION  dans le fichier ===>"<<name<<"<=== Problème à la ligne: "<<nb_ligne<<endl; 
		    cout<<"\nRequired format for "<<name<<endl;
		    cout<<" start_frequency:  W, (W is a uint) "<<endl;
		    cout<<" end_frequency:    X, (X is a uint)"<<endl;
		    cout<<" dmips:            Y, (Y is a flaot) "<<endl;
		    cout<<" pipeline:         Z, (Z is a uint) "<<endl;
		    cout<<" \nExample cortexA8.txt \n\tstart_frequency: 100\n\tend_frequency: 500\n\tdmips: 1.98\n\tpipeline: 11 \n"<<endl;
		    exit(3);
	      }
	    } 
	  }
       }
    }/// end while()..
  }
  else {cout<<"\nERREUR: Impossible d'ouvrir: "<<name<<endl<<endl;exit(0);}
  
  if(!(freq>=newcpu->start_frequency && freq<=newcpu->end_frequency)) 
  {cout<<"pour le "<<name<<" la plage frequentielle doit être compris entre :["<<newcpu->start_frequency<<","<<newcpu->end_frequency<<"]"<<endl; exit(3);}
  else newcpu->work_frequency = freq;
  cpu_file.close();
  newcpu->next = NULL;
  if(mon_cpu==NULL)return newcpu;
  else 
  {
    cpu *cpu_temp=mon_cpu; 
    while(cpu_temp->next!=NULL) cpu_temp = cpu_temp->next;
    cpu_temp->next= newcpu;
    return mon_cpu;
  }
}

DSP ADD_DSP(DSP mon_dsp, string name,uint freq)
{
  dsp *newdsp = new dsp;
  int nb_ligne=0;;
  string line, tmp;
  ifstream dsp_file(name.c_str());
  /// on assigne les parametres du nouveau dsp
  if(dsp_file)
  {
     while(dsp_file>>line ){ /// on lit mot par mot le fichier et on instantie les composants
      nb_ligne++;
      dsp_file>>tmp;
       if(line=="start_frequency:") newdsp->start_frequency =atoi(tmp.c_str());
       else{
	  if(line=="end_frequency:") newdsp->end_frequency =atoi(tmp.c_str());
	  else{
	    if(line=="dmips:") newdsp->dmips =atof(tmp.c_str());
	    else{
	      if(line=="pipeline:") newdsp->pipeline =atoi(tmp.c_str());
	      else{
		    nb_ligne--;
		    cout <<" \n\n ATTENTION  dans le fichier ===>"<<name<<"<=== Problème à la ligne: "<<nb_ligne<<endl; 
		    cout<<"\nRequired format for "<<name<<endl;
		    cout<<" start_frequency:  W, (W is a uint) "<<endl;
		    cout<<" end_frequency:    X, (X is a uint)"<<endl;
		    cout<<" dmips:            Y, (Y is a flaot) "<<endl;
		    cout<<" pipeline:         Z, (Z is a uint) "<<endl;
		    cout<<" \nExample dspX.txt \n\tstart_frequency: 100\n\tend_frequency: 500\n\tdmips: 1.98\n\tpipeline: 11 \n"<<endl;
		    exit(3);
	      }
	    } 
	  }
       }
    }/// end while()..
  }
  else {cout<<"\nERREUR: Impossible d'ouvrir: "<<name<<endl<<endl;exit(0);}
  
  if(!(freq>=newdsp->start_frequency && freq<=newdsp->end_frequency)) 
  {cout<<"pour le "<<name<<" la plage frequentielle doit être compris entre :["<<newdsp->start_frequency<<","<<newdsp->end_frequency<<"]"<<endl; exit(3);}
   else newdsp->work_frequency = freq;

  dsp_file.close();
  newdsp->next = NULL;
  if(mon_dsp==NULL)return newdsp;
  else 
  {
    dsp *dsp_temp=mon_dsp; 
    while(dsp_temp->next!=NULL) dsp_temp = dsp_temp->next;
    dsp_temp->next= newdsp;
    return mon_dsp;
  }
}


CACHE ADD_CACHE(CACHE ma_cache,uint taille, uint assoc, uint bpl)
{
  cache *newcache = new cache;
  /// on assigne les parametres du nouveau cache
  newcache-> taille=taille;
  newcache->associativity =assoc; 
  newcache->bpl = bpl;
  newcache->next = NULL;
  if(ma_cache==NULL){ return newcache;}
  else 
  {
    cache *cache_temp=ma_cache; 
    while(cache_temp->next!=NULL) cache_temp = cache_temp->next;
 
    cache_temp->next= newcache;
    return ma_cache;
  }
}

COMPONENTCPU ADD_COMPONENTCPU(COMPONENTCPU my_component,CPU mon_cpu,CACHE ma_cache,uint indice_cache,uint indice_cpu, string link)
{
  componentcpu *new_component= new componentcpu;
  new_component->CPU_=mon_cpu;
  new_component->L1_=ma_cache;
  ma_cache->namelink=link;
  mon_cpu->indice_cache_L1= indice_cache;
  mon_cpu->indice_CPU= indice_cpu;
  new_component->next = NULL;
  if(my_component==NULL) { return new_component;}
  else
  {
      componentcpu *compo_tmp=my_component;
      while(compo_tmp->next!=NULL)compo_tmp=compo_tmp->next;
      
      compo_tmp->next = new_component;
      return my_component;
  }
}


COMPONENTDSP ADD_COMPONENTDSP(COMPONENTDSP my_component,DSP mon_dsp,CACHE ma_cache, uint indice_cache,uint indice_dsp, string link)
{
  componentdsp *new_component= new componentdsp;
  new_component->DSP_=mon_dsp;
  new_component->L1_=ma_cache;
  mon_dsp->indice_cache_L1= indice_cache;
  mon_dsp->indice_DSP= indice_dsp;  
  ma_cache->namelink=link;
  new_component->next = NULL;
  if(my_component==NULL) { return new_component;}
  else
  {
    componentdsp *compo_tmp=my_component;
    while(compo_tmp->next!=NULL)compo_tmp=compo_tmp->next;

    compo_tmp->next = new_component;
    return my_component;
  }
}


void ADD_CACHE_L2_TO_COMPONENTCPU(COMPONENTCPU my_component,CACHE L2){my_component->L2_=L2;}
void ADD_CACHE_L2_TO_COMPONENTDSP(COMPONENTDSP my_component,CACHE L2){my_component->L2_=L2;}
string GetNamelinkCache(CACHE ma_cache){if(ma_cache!=NULL) return ma_cache->namelink;}

uint GetLinkCacheL1DSP(COMPONENTDSP my_component,uint indice)
{
  componentdsp *tmp=my_component;
  while(tmp!=NULL)
  {
    if(tmp->DSP_->indice_cache_L1==indice){return tmp->DSP_->indice_DSP;}        
    tmp = tmp->next;
  }
  return NULL;
}


uint GetLinkCacheL1CPU(COMPONENTCPU my_component,uint indice)
{
  componentcpu *tmp=my_component;
  while(tmp!=NULL)
  {
    if(tmp->CPU_->indice_cache_L1==indice){return tmp->CPU_->indice_CPU;}        
    tmp = tmp->next;
  }
  return NULL;
}
 

/// pour avoir acces au i-ème composant
CPU AccessToCPU(CPU mon_cpu, uint indice)
{
 for(uint i=0; i<indice && mon_cpu!=NULL;i++) mon_cpu=mon_cpu->next;
 if(mon_cpu==NULL) return NULL;
 else return mon_cpu;
}

DSP AccessToDSP(DSP mon_dsp, uint indice)
{
 for(uint i=0; i<indice && mon_dsp!=NULL;i++) mon_dsp=mon_dsp->next;
 if(mon_dsp==NULL) return NULL;
 else return mon_dsp;
}

CACHE AccessToCACHE(CACHE ma_cache, uint indice)
{
 for(uint i=0; i<indice && ma_cache!=NULL;i++) ma_cache=ma_cache->next;
 if(ma_cache==NULL) return NULL;
 else return ma_cache;
}
COMPONENTCPU AccessToCOMPONENTCPU(COMPONENTCPU my_component, uint indice)
{
 for(uint i=0; i<indice && my_component!=NULL;i++) my_component=my_component->next;
 if(my_component==NULL) return NULL;
 else return my_component;
}
COMPONENTDSP AccessToCOMPONENTDSP(COMPONENTDSP my_component, uint indice)
{
 for(uint i=0; i<indice && my_component!=NULL;i++) my_component=my_component->next;
 if(my_component==NULL) return NULL;
 else return my_component;
}

int NB_CPU(CPU A){if(A == NULL) return 0; return NB_CPU(A->next)+1;}
int NB_DSP(DSP A){if(A == NULL) return 0; return NB_DSP(A->next)+1;}
int NB_CACHE(CACHE A){if(A == NULL) return 0;return NB_CACHE(A->next)+1; }
int NB_COMPONENTCPU(COMPONENTCPU A){if(A == NULL) return 0; return NB_COMPONENTCPU(A->next)+1; }
int NB_COMPONENTDSP(COMPONENTDSP A){if(A == NULL) return 0; return NB_COMPONENTDSP(A->next)+1; }

void Affiche_CPU(CPU A)
{
    cpu *tmp = A;   
    while(tmp!=NULL)
    {     
      cout<<"start_frequency: "<<tmp->start_frequency<<endl;
      cout<<"working_frequency: "<<tmp->work_frequency<<endl;
      cout<<"end_frequency: "<<tmp->end_frequency<<endl;
      cout<<"dmips: "<<tmp->dmips<<endl;
      cout<<"pipeline: "<<tmp->pipeline<<endl;
      tmp=tmp->next;
      cout<<endl;
    }
}

void Affiche_CACHE(CACHE A)
{
    cache *tmp = A;    
    while(tmp!=NULL)
    {      
      cout<<"taille: "<<tmp->taille<<endl;
      cout<<"associativity: "<<tmp->associativity<<endl;
      cout<<"nb byte per line: "<<tmp->bpl<<endl;
      tmp=tmp->next;
      cout<<endl;
    }
}


void Affiche_COMPONENTCPU(COMPONENTCPU A)
{
  uint i=0;
  componentcpu *tmp=A;
  while(tmp!=NULL)
  {
   cout<<endl;
   //cout<<"composant N°"<<i<<":"<<endl;
   cout<<"cpu_"<<tmp->CPU_->indice_CPU<<": "<<tmp->CPU_->name_cpu<<endl;
   cout<<"\tstart_frequency: "<<tmp->CPU_->start_frequency<<endl;
   cout<<"\tworking_frequency: "<<tmp->CPU_->work_frequency<<endl;
   cout<<"\tend_frequency: "<<tmp->CPU_->end_frequency<<endl;
   cout<<"\tdmips: "<<tmp->CPU_->dmips<<endl;
   cout<<"\tpipeline: "<<tmp->CPU_->pipeline<<endl;
   cout<<"cache L1"<<endl;
   cout<<"\ttaille: "<<tmp->L1_->taille<<endl;
   cout<<"\tassociativity: "<<tmp->L1_->associativity<<endl;
   cout<<"\tnb byte per line: "<<tmp->L1_->bpl<<endl;
   cout<<"cache L2"<<endl;
   cout<<"\ttaille: "<<tmp->L2_->taille<<endl;
   cout<<"\tassociativity: "<<tmp->L2_->associativity<<endl;
   cout<<"\tnb byte per line: "<<tmp->L2_->bpl<<endl;
   cout<<endl;
   tmp=tmp->next;   
  }
 
}
  
void Affiche_COMPONENTDSP(COMPONENTDSP A)
{
  uint i=0;
  componentdsp *tmp=A;
  while(tmp!=NULL)
  {
   cout<<endl;
   //cout<<"composant N°"<<i<<":"<<endl;
   cout<<"dsp_"<<tmp->DSP_->indice_DSP<<": "<<endl;
   cout<<"\tstart_frequency: "<<tmp->DSP_->start_frequency<<endl;
   cout<<"\tworking_frequency: "<<tmp->DSP_->work_frequency<<endl;
   cout<<"\tend_frequency: "<<tmp->DSP_->end_frequency<<endl;
   cout<<"\tdmips: "<<tmp->DSP_->dmips<<endl;
   cout<<"\tpipeline: "<<tmp->DSP_->pipeline<<endl;
   cout<<"cache L1"<<endl;
   cout<<"\ttaille: "<<tmp->L1_->taille<<endl;
   cout<<"\tassociativity: "<<tmp->L1_->associativity<<endl;
   cout<<"\tnb byte per line: "<<tmp->L1_->bpl<<endl;
   cout<<"cache L2"<<endl;
   cout<<"\ttaille: "<<tmp->L2_->taille<<endl;
   cout<<"\tassociativity: "<<tmp->L2_->associativity<<endl;
   cout<<"\tnb byte per line: "<<tmp->L2_->bpl<<endl;
   cout<<endl;
   tmp=tmp->next; 
  }
 
}
  