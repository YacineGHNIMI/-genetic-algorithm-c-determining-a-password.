

#include <iostream>					//  pour les couts 
#include <vector>					//  vector class
#include <string>					//  string class
#include <algorithm>				// algorithme de tri 
#include <time.h>					// fonction random 
#include <math.h>					// fonction abs()

#define GA_POPSIZE		2048		//  taille de la population
#define GA_MAXITER		15000		// nombre maximum d'iterations
#define GA_ELITRATE		0.10f		// elitism rate 10%
#define GA_MUTATIONRATE	0.25f		// mutation rate 25%
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("borhane123 /lmzdjfbmajldfg 123fd   123312") // le mot de passe à chercher

using namespace std; 

struct ga_struct 
{
	string str;						// chaine de caractere
	unsigned int fitness;			//  fitness de la chaine
};

typedef vector<ga_struct> ga_vector;

void init_population(ga_vector &population,
					 ga_vector &buffer ) 
{
	int tsize = GA_TARGET.size();

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen; //citoyen
		
		citizen.fitness = 0; // initialisation à 0
		citizen.str.erase(); //chaine vide

		for (int j=0; j<tsize; j++)
			citizen.str += (rand() % 91) + 32; //  code asccii 32 , 123  

		population.push_back(citizen);  // remplir la chaine aléatoirement par tsize caractéres
	}

	buffer.resize(GA_POPSIZE); // taille de buffer egale à taille de populaiton
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;

	for (int i=0; i<GA_POPSIZE; i++) {
		fitness = 0;
		for (int j=0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j])); // fonction fitness 
		}
		
		population[i].fitness = fitness;
	}
}

bool fitness_sort(ga_struct x, ga_struct y) 
{ return (x.fitness < y.fitness); } 

inline void sort_by_fitness(ga_vector &population)
{ sort(population.begin(), population.end(), fitness_sort); } // trier les citoyens selon leur fitness 



void elitism(ga_vector &population,     //fonction elitisme 
				ga_vector &buffer, int esize )
{
	for (int i=0; i<esize; i++) {
		buffer[i].str = population[i].str;   // les elites sont copiés dans buffer
		buffer[i].fitness = population[i].fitness;
	}
}


void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size(); //taille du mot de passe
	int ipos = rand() % tsize; // position de mutation
	int delta = (rand() % 91) + 32;  //caractere apres mutation 

	member.str[ipos] = delta ; // mutation
}

void cross(ga_vector &population, ga_vector &buffer) //fonction de croisement
{
	int esize = GA_POPSIZE * GA_ELITRATE; //10%
	int tsize = GA_TARGET.size(), spos, i1, i2; // taille du mot de passe

	elitism(population, buffer, esize);//10% de la populaiton choisi par elitisme

	// croisement du reste de la population
	for (int i=esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].str = population[i1].str.substr(0, spos) + 
			            population[i2].str.substr(spos, tsize-spos );

		if (rand() < GA_MUTATION) mutate(buffer[i]);// mutation avec une porobabilité 25%
	}
}

inline void print_best(ga_vector &gav) //affiche meilleur element et sa  fitness  (fitness la plus petite)
{ cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << endl; }

inline void swap(ga_vector *&population,
				 ga_vector *&buffer) // charger la nouvelle population 
{ ga_vector *temp = population; population = buffer; buffer = temp; }

int main()
{
	srand(unsigned(time(NULL))); //random

	ga_vector pop_alpha, pop_beta; //declaration de la population 
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta); // initialisation
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i=0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// EVALUATION : calculer fitness
		sort_by_fitness(*population);	// tri par fitness croissant
		print_best(*population);		// afficher le meilleur 

		if ((*population)[0].fitness == 0) break; // arreter si le mot de passe est trouvé

		cross(*population, *buffer);		// contient la SELECTION par elitisme , puis CROISEMENT et MUTATION 
		swap(population, buffer);		// nouvelle generation 
	}

	return 0;
}
