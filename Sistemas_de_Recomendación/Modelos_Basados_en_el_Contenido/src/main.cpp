#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>

#include "../include/functions.hpp"

int main(int argc, char *argv[]) {
  std::cout << "Comenzando el programa" << std::endl;
  std::string nombre_archivo = argv[1];

  std::ifstream fi(nombre_archivo);
  std::ifstream fi_aux(nombre_archivo);
  std::ifstream fi_aux_1(nombre_archivo);

  if (!fi || !fi_aux || !fi_aux_1) {
    throw "Error de apertura del archivo de entrada";
  }

  int num_doc = num_docs(fi);

  std::vector<std::vector<std::string>> mat_docs_rep = calc_m_term_rep(fi_aux);
  std::vector<std::vector<std::string>> mat_docs_no_rep = calc_m_term_no_rep(fi_aux_1);
  std::vector<std::vector<std::pair<std::string, double>>> mat_frec_pal;

  mat_frec_pal.resize(mat_docs_no_rep.size());
  for (size_t i = 0; i < mat_frec_pal.size(); i++) {
    mat_frec_pal[i].resize(mat_docs_no_rep[i].size() + 1);
  }

  const char separador = ' ';
  const int tam = 12;

  for (size_t i = 0; i < mat_docs_no_rep.size(); i++) {
    std::cout << "DOCUMENTO: " << i << "\n\n";
    std::cout << "INDICE" << std::setw(tam) << std::setfill(separador) << 
    "TÉRMINO" << std::setw(7) << std::setfill(separador) <<
    "TF" << std::setw(tam) << std::setfill(separador) <<
    "IDF" << std::setw(tam) << std::setfill(separador) <<
    "TF-IDF" << std::endl;
    for (size_t j = 0; j < mat_docs_no_rep[i].size(); j++) {
      std::cout << j << std::setw(tam) << std::setfill(separador);
      std::cout << mat_docs_no_rep[i][j] << std::setw(tam) << std::setfill(separador);
      int tf = num_term_rep(mat_docs_rep[i], mat_docs_no_rep[i][j]);
      //un vector por cada documento, para normalizar tf
      std::cout << tf << std::setw(tam) << std::setfill(separador);
      int df = num_veces_docs(mat_docs_no_rep, mat_docs_no_rep[i][j]);
      double div = (double)num_doc / (double)df;
      double idf = log(div);
      std::cout << idf << std::setw(tam) << std::setfill(separador);
      double tf_idf = tf * idf;
      std::cout << tf_idf << std::setw(tam) << std::setfill(separador);
      mat_frec_pal[i][j].first = mat_docs_no_rep[i][j];
      mat_frec_pal[i][j].second = tf; 
      std::cout << "\n";
    }
    std::cout << "\n\n\n";
  }

  for (size_t i = 0; i < mat_frec_pal.size(); i++) {
    double long_vect = 0;
    for (size_t j = 0; j < mat_frec_pal[i].size() - 1; j++) {
      long_vect += pow(mat_frec_pal[i][j].second, 2);
    }
    long_vect = sqrt(long_vect);
    mat_frec_pal[i][mat_frec_pal[i].size() - 1].first = "longitud: ";
    mat_frec_pal[i][mat_frec_pal[i].size() - 1].second = long_vect;
  }

  //std::cout << "MATRIZ DE FRECUENCIAS: " << "\n\n";

  /*for (size_t i = 0; i < mat_frec_pal.size(); i++) {
    std::cout << "DOCUMENTO: " << i << "\n\n";
    for (size_t j = 0; j < mat_frec_pal[i].size(); j++) {
      std::cout << j << " -- " << mat_frec_pal[i][j].first << " -- " << mat_frec_pal[i][j].second << std::endl;
    }
    std::cout << "\n\n\n";
  }*/

  for (size_t i = 0; i < mat_frec_pal.size(); i++) {
    double long_vect = mat_frec_pal[i][mat_frec_pal[i].size() - 1].second;
    for (size_t j = 0; j < mat_frec_pal[i].size() - 1; j++) {
      mat_frec_pal[i][j].second = mat_frec_pal[i][j].second / long_vect;
    }
  }   

  //std::cout << "MATRIZ DE FRECUENCIAS NORMALIZADAS: " << "\n\n";

  /*for (size_t i = 0; i < mat_frec_pal.size(); i++) {
    std::cout << "DOCUMENTO: " << i << "\n\n";
    for (size_t j = 0; j < mat_frec_pal[i].size(); j++) {
      std::cout << j << " -- " << mat_frec_pal[i][j].first << " -- " << mat_frec_pal[i][j].second << std::endl;
    }
    std::cout << "\n\n\n";
  }*/

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

  return 0;
}
