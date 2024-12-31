#include <Rcpp.h>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
using namespace Rcpp;

// Structure to hold configuration data
struct Config {
  std::map<std::string, std::map<std::string, std::string>> sections;
};

// Function to write the configuration data to an INI file
void write_ini_file(const std::string& filename, const Config& config) {
  std::ofstream ini_file(filename);
  if (!ini_file.is_open()) {
    Rcpp::stop("Failed to open file: " + filename);
  }

  for (const auto& section : config.sections) {
    ini_file << "[" << section.first << "]\n";
    for (const auto& pair : section.second) {
      ini_file << pair.first << " = " << pair.second << "\n";
    }
    ini_file << "\n";
  }

  ini_file.close();
  if (!ini_file.good()) {
    Rcpp::stop("Failed to write to file: " + filename);
  }
}

//' Write Configuration Data to an INI File
//'
//' This function converts an R list structure into an INI file format and writes it to a specified file.
//'
//' @param config_list A named list where each element is itself a named list representing sections and key-value pairs of the configuration data.
//' @param filename A string specifying the path to the output INI file.
//'
//' @details
//' The `config_list` parameter should be a named list where each top-level name represents a section in the INI file.
//' Each section should be a named list where the names are the keys and the values are the corresponding values in the INI file.
//' Numeric values are converted to strings before being written to the file.
//'
//' If the `config_list` is empty, an empty INI file will be created.
//'
//' @return
//' This function does not return a value. It writes the INI file to the specified path.
//'
//' @examples
//' config <- list(
//'   section1 = list(
//'     key1 = "value1",
//'     key2 = 123
//'   ),
//'   section2 = list(
//'     keyA = "valueA",
//'     keyB = "valueB"
//'   )
//' )
//' ini_file <- tempfile()
//' write_ini(config, ini_file)
//' unlink(ini_file)
//'
//' @useDynLib inih _inih_write_ini
//' @importFrom Rcpp evalCpp
//' @export
// [[Rcpp::export]]
void write_ini(Rcpp::List config_list, std::string filename) {
  Config config;
  // Check if the list is empty
  if (config_list.size() == 0) {
    write_ini_file(filename, config);
    return;
  }
  // Convert Rcpp::List to Config structure
  Rcpp::CharacterVector section_names = config_list.names();
  for (int i = 0; i < config_list.size(); ++i) {
   std::string section_name = Rcpp::as<std::string>(section_names[i]);
   Rcpp::List section_list = config_list[i];
   Rcpp::CharacterVector names = section_list.names();

   for (int j = 0; j < section_list.size(); ++j) {
     std::string name = Rcpp::as<std::string>(names[j]);
     std::string value;
     if (Rf_isNumeric(section_list[j])) {
       std::ostringstream oss;
       oss << Rcpp::as<double>(section_list[j]);
       value = oss.str();
     } else {
       value = Rcpp::as<std::string>(section_list[j]);
     }
     config.sections[section_name][name] = value;
   }
 }

 write_ini_file(filename, config);
}
