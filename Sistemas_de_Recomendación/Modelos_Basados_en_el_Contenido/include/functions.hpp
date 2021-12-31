#include <vector>
#include <string>
#include <fstream>

bool encontrado(std::vector<std::string>& vector_terminos_no_rep, std::string elemento) {
  for (size_t i = 0; i < vector_terminos_no_rep.size(); i++) {
    if (vector_terminos_no_rep[i] == elemento) {
      return true;
    }
  }
  return false;
}

int num_docs(std::ifstream& fi) {
  std::string str = "";
  int num_docs = 0;

  while (getline(fi, str)) {
    num_docs++;   
  }

  return num_docs;
}

int num_term_rep(std::vector<std::string>& vect_term_rep, std::string elemento) {
  int frecuencia = 0;
  for (size_t i = 0; i < vect_term_rep.size(); i++) {
    if (vect_term_rep[i] == elemento) {
      frecuencia++;
    }
  }
  return frecuencia;
}

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

bool ind_siml_enct(int x, int y, std::vector<std::pair<int, int>>& vect) {
  for (size_t i = 0; i < vect.size(); i++) {
    if (x == vect[i].second && y == vect[i].first) {
      return true;
    }
  }
  return false;
}

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