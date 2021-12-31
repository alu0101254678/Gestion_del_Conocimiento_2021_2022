# __Sistemas de Recomendación - Modelos basados en el contenido__
## __Yago Pérez Molanes (alu0101254678@ull.edu.es)__

### __Descripción del código desarrollado.__
En esta práctica se pretende ilustrar el funcionamiento de un sistema recomendador basado en el contenido, para ello, se ha 
creado una estructura de directorios adecuada para el funcionamiento de, en este caso, un programa en C++.

En el directorio */bin* se encuentra el programa ejecutable, mientras que en *./documents* están los archivos de prueba
en los que se alojan los documentos que vienen representados por cada línea del archivo.

Posteriormente en *./include*, está el archivo de declaración de funciones que se necesita para el programa principal de código 
fuente, en ./src, que sería *main.cpp*.

En concreto, se detalla el contenido de los códigos de *funcions.hpp*, y *main.cpp*:

En ***functions.hpp*** se pueden apreciar las siguientes funciones:

```C++
int num_docs(std::ifstream& fi) {
  std::string str = "";
  int num_docs = 0;

  while (getline(fi, str)) {
    num_docs++;   
  }

  return num_docs;
}
```
Esta función, *num_docs*, cuenta el número de documentos en un archivo determinado.

```C++
int num_term_rep(std::vector<std::string>& vect_term_rep, std::string elemento) {
  int frecuencia = 0;
  for (size_t i = 0; i < vect_term_rep.size(); i++) {
    if (vect_term_rep[i] == elemento) {
      frecuencia++;
    }
  }
  return frecuencia;
}
```
Esta función, calcula la frecuencia con la que aparece un término en un documento. Cabe
destacar que se usa una matriz, o un vector de vectores, para representar las palabras de 
cada documento, es decir, que se tiene un vector de documentos, y cada elemento del vector
contiene a su vez otro vector con las palabras, no importa que sean repetidas.

Sin embargo, también se tiene otro vector de vectores que contiene las palabras que aparecen
una única vez en cada documento.

Por otro lado se encuentran dos funciones que son las más importantes, una que calcula la matriz
de términos repetidos, y otra que calcula la matriz de términos no repetidos.

```C++
std::vector<std::vector<std::string>> calc_m_term_rep(std::ifstream& fi_aux) {
  std::string documento = "";
  std::vector<std::vector<std::string>> matriz_terminos_rep;
  while (getline(fi_aux, documento)) {
    std::vector<std::string> vector_terminos_rep;
    std::string documento_auxiliar = "";
    for (size_t i = 0; i < documento.size(); i++) {
      documento[i] = tolower(documento[i]);
      if (documento[i] == ',' || documento[i] == '.') {
        documento[i] = ' ';
        documento_auxiliar += documento[i];
      } else {
        documento_auxiliar += documento[i];
      }
    }

    std::stringstream linestream(documento_auxiliar);
    std::string documento_filtrado;

    
    while (getline(linestream, documento_filtrado, ' ')) {
      if (documento_filtrado.size() != 0) {
        vector_terminos_rep.push_back(documento_filtrado);  
      }
    }
    matriz_terminos_rep.push_back(vector_terminos_rep);
    vector_terminos_rep.clear();
  }
  return matriz_terminos_rep;
}
```
En el caso de la matriz de términos repetidos, se crea un vector de vectores de strings o cadenas de
caracteres para alojar todas las palabras, se lee cada línea del archivo, se quitan los caracteres que
no hacen falta, como las comas y los puntos, y los espacios sobrantes, para que entre cada palabra
solamente quede un espacio, después se vuelve a analizar la línea desde dentro con *getline*, se añaden
las palabras al vector, y ese vector se introduce en la matriz.

Para el caso de los términos que no se repiten el proceso es similar:

```C++
std::vector<std::vector<std::string>> calc_m_term_no_rep(std::ifstream& fi_aux) {
  std::string documento = "";
  std::vector<std::vector<std::string>> matriz_solo_terminos_no_rep;
  while (getline(fi_aux, documento)) {
    std::vector<std::string> vector_terminos_no_rep;
    std::string documento_auxiliar = "";
    for (size_t i = 0; i < documento.size(); i++) {
      documento[i] = tolower(documento[i]);
      if (documento[i] == ',' || documento[i] == '.') {
        documento[i] = ' ';
        documento_auxiliar += documento[i];
      } else {
        documento_auxiliar += documento[i];
      }
    }

    std::stringstream linestream(documento_auxiliar);
    std::string documento_filtrado;

    //std::cout << documento_auxiliar << std::endl;
    while (getline(linestream, documento_filtrado, ' ')) {
      // std::cout << documento_filtrado << std::endl;
      
      if (documento_filtrado.size() != 0 && !encontrado(vector_terminos_no_rep, documento_filtrado)) {
        vector_terminos_no_rep.push_back(documento_filtrado);  
      }
    }
    matriz_solo_terminos_no_rep.push_back(vector_terminos_no_rep);
    vector_terminos_no_rep.clear();
  }
  return matriz_solo_terminos_no_rep;
}
```
Cuando se va a analizar esa línea que está *filtrada*, se tiene que comprobar que esa palabra, el string,
no esté en el vector de términos no repetidos, de ahí que se haga uso de una función auxiliar, 
*encontrado*, al que se le pasa como parámetros el vector que comentábamos, y la palabra que se quiere 
comrpobar, tiene la siguiente estructura:

```C++
bool encontrado(std::vector<std::string>& vector_terminos_no_rep, std::string elemento) {
  for (size_t i = 0; i < vector_terminos_no_rep.size(); i++) {
    if (vector_terminos_no_rep[i] == elemento) {
      return true;
    }
  }
  return false;
}
```
Lo único que hace es comprobar dentro del vector si esa palabra se encuentra o no, y retorna verdadero o
falso seúgn lo haya encontrado o no.

Otra función que se comenta es la que permite calcular el número de documentos en los que aparece un 
determinado término, sirve para calcular el *idf*, *num_veces_docs:*

```C++
int num_veces_docs(std::vector<std::vector<std::string>>& mat_term_no_rep, std::string termino) {
  int contador = 0;

  for (size_t i = 0; i < mat_term_no_rep.size(); i++) {
    for (size_t j = 0; j < mat_term_no_rep[i].size(); j++) {
      if (mat_term_no_rep[i][j] == termino) {
        contador++;
      }
    }
  }
  return contador;
}
```
Para ello se usa la matriz de términos no repetidos calculada anteriormente.

Pasando ahora al programa principal, ***main.cpp***, se realizan las llamadas correspondientes a todas estas
funciones definidas, sin embargo, destacar que para calcular la similitud coseno entre cada par de 
documentos, se emplea una matriz de frecuencias de palabras, ya que solo hace falta el *tf*, es decir
la frecuencia, no es necesario volver a emplear el *idf*, o el *tf-idf*.

```C++
std::vector<std::vector<std::pair<std::string, double>>> mat_frec_pal;

mat_frec_pal.resize(mat_docs_no_rep.size());
for (size_t i = 0; i < mat_frec_pal.size(); i++) {
  mat_frec_pal[i].resize(mat_docs_no_rep[i].size() + 1);
}
```
Sobre esta matriz es en la que se va a basar el calcula de las similitudes, por ello, se crea un vector
de vectores en donde cada elemento es un par, un string con la *palabra*, y un *double*, con la frecuencia,
primero la frecuencia, y después la frecuencia normalizada, destacar que cada posición del vector más externo,
es decir el vector que contiene las palabras, tiene una posición más porque así se puede calcular la longitud
del vector, para poder así calcular los valores normalizados.

Una vez que se muestran por pantalla el índice del término, el propio término, el *tf*, el *idf*, y el *tf-idf*,
para cada documento, se procede a calcular la similitud coseno.

Primero se calcula la longitud de cada vector dentro de la matriz:

```C++
for (size_t i = 0; i < mat_frec_pal.size(); i++) {
  double long_vect = 0;
  for (size_t j = 0; j < mat_frec_pal[i].size() - 1; j++) {
    long_vect += pow(mat_frec_pal[i][j].second, 2);
  }
  long_vect = sqrt(long_vect);
  mat_frec_pal[i][mat_frec_pal[i].size() - 1].first = "longitud: ";
  mat_frec_pal[i][mat_frec_pal[i].size() - 1].second = long_vect;
}
```
Se calcula como la raíz cuadrada de la suma de los valores de las frecuencias de cada palabra al cuadrado en el
vector.

```C++
for (size_t i = 0; i < mat_frec_pal.size(); i++) {
  double long_vect = mat_frec_pal[i][mat_frec_pal[i].size() - 1].second;
  for (size_t j = 0; j < mat_frec_pal[i].size() - 1; j++) {
    mat_frec_pal[i][j].second = mat_frec_pal[i][j].second / long_vect;
  }
}  
```
Una vez calculadas las longitudes se normalizan los valores de cada vector de la matriz de frecuencias, dividiendo
cada valor entre la longitud, es decir, entre la última posición del vector como teníamos estructurado previamente.

Por último ya se procede a calcular las similitudes y mostrarlas por pantalla:

```C++
std::cout << "SIMILARIDAD COSENO ENTRE ARTÍCULOS: " << "\n\n";
std::vector<std::pair<int, int>> aux;

for (size_t i = 0; i < mat_frec_pal.size(); i++) {
  for (size_t j = 0; j < mat_frec_pal.size(); j++) {
    if (i != j && !ind_siml_enct(i, j, aux)) {
      double res = calc_sim(i, j, mat_frec_pal);
      std::cout << "Artículos " << i << " -> " << j << ": " << res << std::endl; 
    }
    aux.push_back(std::make_pair(i, j));
  }
}
```
Se recorre el vector de vectores y se hace uso de dos funciones disponibles en ***functions***, *ind_siml_enct*, y
*calc_sim*, la primera de ellas averigua si ya se ha calculado la similitud entre ese par de documentos, ya que, se 
puede dar el caso de que se esté calculando la similitud entre el documento 5 y el documento 3, cuando ya se ha
calculado la del documento 3 y el documento 5, que será el mismo, por eso se añade el pair al vector, porque
ya se ha calculado esa similitud.

La otra función realiza el cálculo de la similitud entre dos documentos:
```C++
double calc_sim(int x, int y, std::vector<std::vector<std::pair<std::string, double>>>& mat_frec_pal) {
  double resultado = 0;
  for (size_t i = 0; i < mat_frec_pal[x].size() - 1; i++) {
    for (size_t j = 0; j < mat_frec_pal[y].size() - 1; j++) {
      if (mat_frec_pal[x][i].first == mat_frec_pal[y][j].first) {
        resultado += mat_frec_pal[x][i].second * mat_frec_pal[y][j].second;
      }
    }
  }
  return resultado;
}
```
Solo hay que comprobar en los dos vectores las palabras que sean iguales y realizar el calculo.

### __Ejemplo de uso.__
El programa se usa de la siguiente manera: (destacar que la compilación se realiza con un archivo Makefile),
para que sea más sencillo y de forma automática, si se quiere compilar de nuevo, con el comando *make* sería
suficiente.

* **./bin/main ./documents/xxx**
donde *xxx* es el nombre del archivo en el que se encuentran los documentos.

Un ejemplo de uso sería el siguiente:
* **./bin/main ./documents/example_1.txt**

El programa mostraría la siguiente salida por la terminal, para el primer archivo que contiene 10 documentos:

Estas capturas corresponden a los valores de la frecuencia, el idf, el tf-idf, así como de el índice y los términos
de los documentos 0, 2, 8, y 9, mostrados en una tabla.

![captura_1](/Sistemas_de_Recomendación/Modelos_Basados_en_el_Contenido/images/captura_1.png)

![captura_2](/Sistemas_de_Recomendación/Modelos_Basados_en_el_Contenido/images/captura_2.png)

![captura_3](/Sistemas_de_Recomendación/Modelos_Basados_en_el_Contenido/images/captura_3.png)

La salida correspondiente a la similaridad coseno entre cada par de documentos sería la siguiente:

![captura_4](/Sistemas_de_Recomendación/Modelos_Basados_en_el_Contenido/images/captura_4.png)
