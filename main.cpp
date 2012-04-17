/* hello word*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "structureArchi.h"
using namespace std;

void CreateFile(char *output,string input,float TBC)
{
  ofstream outputfile(output);/// ios::app pour rajouter en fin du fichier ce qu'on veut ecrire 
  ///si on ne veut pas ecraser le fichier dans le cas où il existe déjà
  string line;
  size_t pos;
  ostringstream oss;
  oss<<TBC;
  if(outputfile)
  {
    ifstream inputfile(input.c_str());
    if(inputfile)
    {
      while(getline(inputfile,line)) 
      {
	
	  pos=line.find("TBC");
	  if(pos<=line.size()) line.replace(pos,3, oss.str());
	  outputfile<<line<<endl;
	 // cout<<line<<endl;
      }
    }
    else cout<<"ERREUR: Impossible d'ouvrir le fichier:"<<input<<endl;
  }
  else cout<<"ERREUR: Impossible d'ouvrir le fichier:"<<output<<endl; 
}

int main(int argc, char *argv[])
{
 
  int nb_cpu=0, nb_cache=0, nb_dsp=0,nb_ligne=0;
  string line, filename,tmp; 
  char dir[100];
  CPU MonCpu=NULL,cpu_tmp=NULL;
  DSP MonDsp=NULL,dsp_tmp=NULL;
  CACHE MesCacheL1=NULL,MesCacheL2=NULL,cacheL1_tmp=NULL,cacheL2_tmp=NULL;
  COMPONENTCPU MesComposantsCPU=NULL,CompoCPU_temp=NULL;
  COMPONENTDSP MesComposantsDSP=NULL,CompoDSP_temp=NULL;

  ifstream listing_file("composition.txt");   
 
  if(listing_file)
  {
    uint freq=0, taille=0,assoc=0,bpl=0;
    uint indice_cpu=0,indice_cacheL1=0,indice_cacheL2=0,indice_dsp=0;
    
    while(listing_file>>line){ /// on lit mot par mot le fichier et on instantie les composants
      
      nb_ligne++;
      if(line == "component") 
      {
	listing_file>>line;
	if(line=="PU")
	{
	    listing_file>>line;
	    listing_file>>filename;
	    listing_file>>freq;
	    MonCpu=ADD_CPU(MonCpu,filename,freq);
	}
	else{
	    if(line=="CACHE")
	    {
	      listing_file>>line;
	      tmp = line.substr(0,line.size()-2);	      	      
	      listing_file>>taille;
	      listing_file>>assoc;
	      listing_file>>bpl;
	      if(tmp =="L1") MesCacheL1=ADD_CACHE(MesCacheL1,taille,assoc,bpl);
	      else {if(tmp =="L2") MesCacheL2=ADD_CACHE(MesCacheL2,taille,assoc,bpl);}
	    }
	    else{
	      if(line =="DSP")
	      {
		listing_file>>line;
		listing_file>>filename;
		listing_file>>freq;
		MonDsp=ADD_DSP(MonDsp,filename,freq);
	      }
	    }
	}
	
      }/// end if component
      else{
	  if(line =="connection")
	  {
	      listing_file>>line;
	      tmp = line.substr(0,line.size()-1);
	      if(tmp =="CPU")
	      {
		//cout<<"connnection cpu_";
		indice_cpu=atoi(line.substr(tmp.size(),line.size()).c_str());
		//cout<<indice_cpu;
		listing_file>>line;
		listing_file>>line;
		tmp=line.substr(0,line.size()-1);
		if(tmp=="L1_")
		{
		  //cout<<" to cacheL1_";
		  
		  indice_cacheL1=atoi(line.substr(tmp.size(),line.size()).c_str());

		  MesComposantsCPU = ADD_COMPONENTCPU(MesComposantsCPU,AccessToCPU(MonCpu,indice_cpu),AccessToCACHE(MesCacheL1,indice_cacheL1),indice_cacheL1,indice_cpu,"cpu");
		  //cout<<indice_cacheL1<<endl;   
		}
	      }
	      else {
		if(tmp =="DSP")
		{
		   // cout<<"connnection dsp_";
		    indice_dsp=atoi(line.substr(tmp.size(),line.size()).c_str());
		    //cout<<indice_dsp;
		    listing_file>>line;
		    listing_file>>line;
		    tmp=line.substr(0,line.size()-1);
		      if(tmp=="L1_")
		      {
			//cout<<" to cacheL1_";
			
			indice_cacheL1=atoi(line.substr(tmp.size(),line.size()).c_str());
			MesComposantsDSP = ADD_COMPONENTDSP(MesComposantsDSP,AccessToDSP(MonDsp,indice_dsp),AccessToCACHE(MesCacheL1,indice_cacheL1),indice_cacheL1,indice_dsp,"dsp");
			//cout<<indice_cacheL1<<endl;   
		      }
		}
	      else{
		if(tmp=="L1_")
		{
		  //cout<<"connection cacheL1_";
		  indice_cacheL1=atoi(line.substr(tmp.size(),line.size()).c_str());
// 		  cout<<indice_cacheL1;
		  listing_file>>line;
		  listing_file>>line;
		  tmp=line.substr(0,line.size()-1);
		  if(tmp=="L2_")
		  {
		    
// 		      cout<<" to cacheL2_";
		      indice_cacheL2=atoi(line.substr(tmp.size(),line.size()).c_str());
		      cacheL1_tmp=AccessToCACHE(MesCacheL1,indice_cacheL1);
		      cacheL2_tmp=AccessToCACHE(MesCacheL2,indice_cacheL2);

		      if(GetNamelinkCache(cacheL1_tmp) =="dsp"){
		      CompoDSP_temp=AccessToCOMPONENTDSP(MesComposantsDSP,GetLinkCacheL1DSP(MesComposantsDSP,indice_cacheL1));
		      ADD_CACHE_L2_TO_COMPONENTDSP(CompoDSP_temp,cacheL2_tmp);
		
		      }
		      else{
			if(GetNamelinkCache(cacheL1_tmp) =="cpu"){
			CompoCPU_temp=AccessToCOMPONENTCPU(MesComposantsCPU,GetLinkCacheL1CPU(MesComposantsCPU,indice_cacheL1));
			ADD_CACHE_L2_TO_COMPONENTCPU(CompoCPU_temp,cacheL2_tmp);
			}
		      }
// 		    cout<<indice_cacheL2<<endl;
		  }
		}
	      
	      }
	    }
	  }
	   else 
	   {
	    nb_ligne--;
	    cout <<" \n\n ATTENTION ===> Problème à la ligne: "<<nb_ligne<<" <==="<<endl; 
	    listing_file.seekg(-listing_file.tellg(),ios::cur);
	    while(nb_ligne>0){getline(listing_file,line);nb_ligne--;}
	    cout<<line <<" :Cette ligne n'est pas correcte \n \n"<<endl;
	    exit(0);
	    
	      if(line.substr(0,9)=="component")
	      {

		if(line.substr(10,2)=="PU"){
		  cout<<" Required format CPU :";
		  cout<<" component PU CPUX file_name.txt freq "<<endl;
		  cout<<" X : cpu number"<<endl;
		  cout<<" file_name.txt : architecture decription texte file"<<endl;
		  cout<<" freq: cpu working frequency"<<endl;
		  cout<<" Example: component PU CPU0 cortexA8.txt 200\n"<<endl;
		  exit(0);
		}
	      else
	      {
		if(line.substr(10,5)=="CACHE"){
		  cout<<" Required format cache memory :";
		  cout<<" component CACHE LX_Y A B C "<<endl; 
		  cout<<" X : cache type 1 or 2"<<endl;
		  cout<<" Y : cache number"<<endl;
		  cout<<" A : cache size "<<endl;
		  cout<<" B : cache associativity "<<endl;
		  cout<<" C : cache nb byte/line "<<endl;
		  cout<<" Example: component CACHE L1_2 32 32 32\n"<<endl;
		  exit(1);
		}
	      }
	      }
	      else{
		if(line.substr(0,9) =="connection"){
		cout<<" Required format connection :";
		cout<<" component CX -> CY"<<endl; 
		cout<<" CX : can be  a PU or a cache memory or RAM ,X mean the component number"<<endl;
		cout<<" CY : only RAM or cache memory ,Y mean the component number"<<endl;
		cout<<" Example: \n\tconnection L1_0 -> L2_0 (link a cache L1 to a cache L2) \n \t";
		cout<<"connection CPU1 -> L1_1 (link a PU to a cache L2)\n"<<endl;
		exit(2);
		}
	      }
	   }

      }
	
      }///end while(listing_file>>line)
    
//   cout<<endl;
  }///end if(listing_file)
  else{
    cout<<"ERREUR: Impossible d'ouvrir le fichier de la liste des architectures"<<endl;
    exit(0);

  }
  
  listing_file.close();

  nb_cpu= NB_CPU(MonCpu);
 
  nb_dsp= NB_DSP(MonDsp);
  nb_cache= NB_CACHE(MesCacheL1);
  nb_cache= NB_CACHE(MesCacheL2);
   
//   cout<<"nb_cpu: "<<nb_cpu<<endl;
//   cout<<"nb_dsp: "<<nb_dsp<<endl;
//   cout<<"nb_cache L1: "<<nb_cache<<endl;
//   cout<<"nb_cache L2: "<<nb_cache<<endl;
//  
//   Affiche_COMPONENTDSP(MesComposantsDSP);  
/*  */Affiche_COMPONENTCPU(MesComposantsCPU);
  /// allocations memoires
  uint i=0;
  float T_RWDATA_cpu[nb_cpu];	//temps passé à lire et écrire des datas
  float T_Inst_cpu[nb_cpu];  	//temps passé dans les instructions (ms)
  float T_Total_cpu[nb_cpu]; 	//temps total
  float T_OneDATA_cpu[nb_cpu];  //ms pour lire ou écrire une data
  float MissPred_cpu[nb_cpu];	//MissPred
  float Pen_L1_cpu[nb_cpu];  	//penalité pour un miss L1
  float Pen_L2_cpu[nb_cpu];  	//penalité pour un miss L2
  CompoCPU_temp = MesComposantsCPU;
  while(i<nb_cpu)
  {
    T_OneDATA_cpu[i]=1.0/(CompoCPU_temp->CPU_->work_frequency*1000.0);
    Pen_L1_cpu[i]=10.0/(CompoCPU_temp->CPU_->work_frequency*1000.0);
    Pen_L2_cpu[i]=100.0/(CompoCPU_temp->CPU_->work_frequency*1000.0);
    T_Inst_cpu[i]=(5086842823.0/(400*5.0))/(CompoCPU_temp->CPU_->work_frequency*CompoCPU_temp->CPU_->dmips*1000.0)+(1913.0/(400*5.0)*Pen_L1_cpu[i])+(802.0/(1500*5.0)*Pen_L2_cpu[i]);
    T_RWDATA_cpu[i]=(587188853.0/(400*5.0)+865167484.0/(400*5.0))*(T_OneDATA_cpu[i])+(587188853.0/(400*5.0)*0.01/100.0*Pen_L2_cpu[i]+(865167484.0/(400*5.0)*0.01/100.0*Pen_L2_cpu[i]))+(587188853.0/(400*5.0)*0.1/100.0*Pen_L1_cpu[i]+865167484.0/(400*5.0)*1.2/100.0*Pen_L1_cpu[i]);
    MissPred_cpu[i]=(77066983.0/(1500*5.0))*CompoCPU_temp->CPU_->pipeline*T_OneDATA_cpu[i];
    T_Total_cpu[i]=MissPred_cpu[i]+T_RWDATA_cpu[i]+T_Inst_cpu[i];
    
    sprintf(dir,"slice_cpu_%d.txt",CompoCPU_temp->CPU_->indice_CPU);
    CreateFile(dir,"slice.txt",T_Total_cpu[i]);
//     cout<<"T_OneDATA_cpu["<<i<<"] = "<<T_OneDATA_cpu[i]<<endl;
//     cout<<"Pen_L1_cpu["<<i<<"] = "<<Pen_L1_cpu[i]<<endl;
//     cout<<"Pen_L2_cpu["<<i<<"] = "<<Pen_L2_cpu[i]<<endl;
//     cout<<"T_Inst_cpu["<<i<<"] = "<<T_Inst_cpu[i]<<endl;
//     cout<<"T_RWDATA_cpu["<<i<<"] = "<<T_RWDATA_cpu[i]<<endl;
//     cout<<"MissPred_cpu["<<i<<"] = "<<MissPred_cpu[i]<<endl;
    cout<<"T_Total_cpu["<<i<<"] = "<<T_Total_cpu[i]<<endl<<endl;
  i++;  
  CompoCPU_temp=CompoCPU_temp->next;

  }
  
  i=0;
  
 /* if(nb_dsp>0)
  {*/
  float T_RWDATA_dsp[nb_dsp];	//temps passé à lire et écrire des datas
  float T_Inst_dsp[nb_dsp];  	//temps passé dans les instructions (ms)
  float T_Total_dsp[nb_dsp]; 	//temps total
  float T_OneDATA_dsp[nb_dsp];  //ms pour lire ou écrire une data
  float MissPred_dsp[nb_dsp];	//MissPred
  float Pen_L1_dsp[nb_dsp];  	//penalité pour un miss L1
  float Pen_L2_dsp[nb_dsp];  	//penalité pour un miss L2
  CompoDSP_temp=MesComposantsDSP;
  while(i<nb_dsp)
  {
    T_OneDATA_dsp[i]=1.0/(CompoDSP_temp->DSP_->work_frequency*1000.0);
    Pen_L1_dsp[i]=10.0/(CompoDSP_temp->DSP_->work_frequency*1000.0);
    Pen_L2_dsp[i]=100.0/(CompoDSP_temp->DSP_->work_frequency*1000.0);
    T_Inst_dsp[i]=(5086842823.0/(400*5.0))/(CompoDSP_temp->DSP_->work_frequency*CompoDSP_temp->DSP_->dmips*1000.0)+(1913.0/(400*5.0)*Pen_L1_dsp[i])+(802.0/(1500*5.0)*Pen_L2_dsp[i]);
    T_RWDATA_dsp[i]=(587188853.0/(400*5.0)+865167484.0/(400*5.0))*(T_OneDATA_dsp[i])+(587188853.0/(400*5.0)*0.01/100.0*Pen_L2_dsp[i]+(865167484.0/(400*5.0)*0.01/100.0*Pen_L2_dsp[i]))+(587188853.0/(400*5.0)*0.1/100.0*Pen_L1_dsp[i]+865167484.0/(400*5.0)*1.2/100.0*Pen_L1_dsp[i]);
    MissPred_dsp[i]=(77066983.0/(1500*5.0))*CompoDSP_temp->DSP_->pipeline*T_OneDATA_dsp[i];
    T_Total_dsp[i]=MissPred_dsp[i]+T_RWDATA_dsp[i]+T_Inst_dsp[i];
    
    sprintf(dir, "slice_dsp_%d.txt",CompoDSP_temp->DSP_->indice_DSP);
    CreateFile(dir,"slice.txt",T_Total_dsp[i]);
  
  
//     cout<<"T_OneDATA_dsp["<<i<<"] = "<<T_OneDATA_dsp[i]<<endl;
//     cout<<"Pen_L1_dsp["<<i<<"] = "<<Pen_L1_dsp[i]<<endl;
//     cout<<"Pen_L2_dsp["<<i<<"] = "<<Pen_L2_dsp[i]<<endl;
//     cout<<"T_Inst_dsp["<<i<<"] = "<<T_Inst_dsp[i]<<endl;
//     cout<<"T_RWDATA_dsp["<<i<<"] = "<<T_RWDATA_dsp[i]<<endl;
//     cout<<"MissPred_dsp["<<i<<"] = "<<MissPred_dsp[i]<<endl;
//     cout<<"T_Total_dsp["<<i<<"] = "<<T_Total_dsp[i]<<endl<<endl;
  i++;  
  CompoDSP_temp=CompoDSP_temp->next;

  }
 /* }*/
 
 /** definition des affiliations    **/
//  exec() X threads sur tel CPU

 

  

/// generation de fichier

//line = MesComposantsCPU->CPU_->name_cpu;
//tmp = line.substr(0,line.size()-4);
   

// // line.erase(); 
// line= itoa(i,line,10);
// line = tmp + line+".txt";
 //sprintf(dir, "slice_cpu_%d.txt",i);
//tmp = tmp+dir;
//cout<<tmp<<endl;
//CreateFile(dir,MesComposantsCPU->CPU_->name_cpu, float _Total_dsp[i]);float TBC

  
   return 0;
}
  
