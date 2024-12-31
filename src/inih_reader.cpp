#include "ini.h"
#include <Rcpp.h>
using namespace Rcpp;

// Structure to hold configuration data
struct Config {
  std::map<std::string, std::map<std::string, std::string>> sections;
};

// Callback function to handle parsed data
static int handler(void* user, const char* section, const char* name, const char* value) {
  Config* config = (Config*)user;
  config->sections[section][name] = value;
  return 1;  // Return non-zero to indicate success
}

//' Read Configuration Data from an INI File
//'
//' This function reads an INI file and converts its contents into an R list structure.
//'
//' @param filename A string specifying the path to the input INI file.
//'
//' @details
//' The `read_ini` function parses the specified INI file and converts its sections
//' and key-value pairs into a nested R list. Each section in the INI file becomes
//' a named list within the result, and each key-value pair within a section becomes
//' an element of that list.
//'
//' If the INI file cannot be loaded, the function will raise an error.
//'
//' @return
//' A named list where each element is itself a named list representing sections
//' and key-value pairs of the configuration data.
//'
//' @examples
//' config <- list(
//'     section1 = list(
//'       key1 = "value1"
//'     ),
//'     section2 = list(
//'       keyA = "valueA",
//'       keyB = "valueB"
//'     )
//' )
//'  ini_file <- tempfile()
//'  write_ini(config, ini_file)
//'  read_ini(ini_file)
//'
//'  unlink(ini_file)
//'
//' @useDynLib inih _inih_read_ini
//' @importFrom Rcpp evalCpp
//' @export
// [[Rcpp::export]]
Rcpp::List read_ini(std::string filename) {
 Config config;

 if (ini_parse(filename.c_str(), handler, &config) < 0) {
   Rcpp::stop("Can't load the INI file");
 }

 Rcpp::List result;
 for (auto const& section : config.sections) {
   Rcpp::List section_list;
   for (auto const& pair : section.second) {
     section_list[pair.first] = pair.second;
   }
   result[section.first] = section_list;
 }

 return result;
}
