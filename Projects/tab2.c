#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char species[16];
    float sepal_lenght, sepal_width, petal_lenght, petal_width;
}flower;


float square(float x){
    return x*x;
}


//calculatesd the euclidean distance between two flowers
float dist(flower f1, flower f2){
    return square(f1.petal_lenght - f2.petal_lenght) + square(f1.petal_width - f2.petal_width) + square(f1.sepal_width - f2.sepal_width) + square(f1.sepal_lenght - f2.sepal_lenght);
}

//gets the id of the specie
int specie_id(char *name){
    if(strcmp(name, "setosa") == 0)
        return 0;
    if(strcmp(name, "versicolor") == 0)
        return 1;
    return 2;
}

//gets the specie from the id
char *id_to_species(int id){
    char *ret[3] = {"setosa", "versicolor", "virginica"};
    return ret[id];
}

//merges two sorted vectors into a third sorted vector
void merge(flower example, flower *dataset, int beg, int end){
  int mid = (end + beg)/2;
  flower *w = malloc((end - beg + 1)*sizeof(flower)); //temporarily stores the merged vectors
  int i = beg, j = mid + 1, k = 0;

  //while you haven't added all the elements of one of the vectors to W
  while(i <= mid && j <= end){
    if(dist(example, dataset[i]) < dist(example, dataset[j])) w[k] = dataset[i++];
    else  w[k]  = dataset[j++];
    ++k;
  }
  //add the leftovers
  for(; i <= mid; ++i, ++k) w[k]  = dataset[i];
  for(; j <= end; ++j, ++k) w[k]  = dataset[j];

  for(i = beg; i <= end; ++i){
    dataset[i] = w[i - beg];
  }
  free(w);
  return;
}

//mergesort function, sorts based on distance to example
void mergesort(flower example, flower *dataset, int beg, int end){
  if(end - beg <= 0)  return;
  int mid = (beg + end)/2;
  mergesort(example, dataset, beg, mid);
  mergesort(example, dataset, mid + 1, end);
  merge(example, dataset, beg, end);
}

//Knn algorithm, returns the guessed specie of the example flower
char *knn(int size, flower* dataset, flower example, int k, float *rate){
    int classes[3] = {0}, max_id = 0;
    float min_dist[3] = {144444, 144444, 144444}; //min_dist to each class initialized to infinity
    flower neighbors[k], aux;

    mergesort(example, dataset, 0, size - 1);

    for(int i = 0; i < k; ++i)  neighbors[i] = dataset[i];

    for(int i = 0; i < k; ++i)
        if(min_dist[specie_id(neighbors[i].species)] > dist(neighbors[i], example))
            min_dist[specie_id(neighbors[i].species)] = dist(neighbors[i], example);


    for(int i = 0; i < k; ++i)
        ++classes[specie_id(neighbors[i].species)];

    for(int i = 0; i < 3; ++i)
        if(classes[max_id] < classes[i])    max_id = i;
        else if(classes[max_id] == classes[i] && (min_dist[max_id] > min_dist[i]))  max_id = i; //desempate

    if(strcmp(id_to_species(max_id), example.species) == 0) *rate += 1;
    return id_to_species(max_id);
}

int main(){
    flower example;
    flower *dataset;
    float rate = 0.0;
    int k, num_examples = 1, num_test = 0, size = 4;
    char lixo, test[32], train[32], discard[5][352];
    FILE *ftest, *ftrain;

    scanf("%s %s %d", train, test, &k);
    ftest = fopen(test, "r");
    ftrain = fopen(train, "r");

    fscanf(ftrain, "%s", discard[0]);

    dataset = calloc(sizeof(flower), 16);
    while(fscanf(ftrain, "%f,%f,%f,%f,\"%s", &dataset[num_examples - 1].sepal_lenght, &dataset[num_examples - 1].sepal_width, &dataset[num_examples - 1].petal_lenght, &dataset[num_examples - 1].petal_width, dataset[num_examples - 1].species) > 0){
        dataset[num_examples - 1].species[strlen(dataset[num_examples - 1].species) - 1] = '\0';

        ++num_examples;
        if(num_examples == size){
            dataset = realloc(dataset, sizeof(flower)*(size + 4));
            size += 4;
        }
    }

    if(k < 1 || k > num_examples){
        printf("k is invalid\n");
        return 0;
    }
    fscanf(ftest, "%s", discard[0]);

    while(fscanf(ftest, "%f,%f,%f,%f,\"%s", &example.sepal_lenght, &example.sepal_width, &example.petal_lenght, &example.petal_width, example.species) > 0){
        ++num_test;
        example.species[strlen(example.species) - 1] = '\0';
        printf("%s %s\n", knn(num_examples, dataset, example, k, &rate), example.species);
    }
    printf("%.4f\n", rate/num_test);
    free(dataset);
    return 0;
}
