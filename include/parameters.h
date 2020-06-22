#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "argparser.h"
#include "definitions.h"
#include "filtration_algorithms.h"

#include <cstring>

/* Avoid name collision with generic name parameters*/
class flagser_parameters {
public:
	flagser_parameters(const named_arguments_t& named_arguments) {
		named_arguments_t::const_iterator it;

		output_name =
		    get_argument_or_fail(named_arguments, "out", "Please provide an output filename via \"--out filename\" .");

		if ((it = named_arguments.find("in-format")) != named_arguments.end()) { input_format = it->second; }

		if ((it = named_arguments.find("out-format")) != named_arguments.end()) { output_format = it->second; }

		if ((it = named_arguments.find("approximate")) != named_arguments.end()) {
			max_entries = atoi(it->second);
			approximate_computation = true;
		}

		directed = std::string(get_argument_or_default(named_arguments, "undirected", "directed")) != "true";

		split_into_connected_components = named_arguments.find("components") != named_arguments.end();

		if ((it = named_arguments.find("max-dim")) != named_arguments.end()) { max_dimension = atoi(it->second); }

		if ((it = named_arguments.find("min-dim")) != named_arguments.end()) { min_dimension = atoi(it->second); }

		if (has_zero_filtration_and_no_explicit_output(named_arguments)) { output_format = "betti"; }

		filtration_algorithm.reset(
		    get_filtration_computer(get_argument_or_default(named_arguments, "filtration", "zero")));

#ifdef USE_COEFFICIENTS
		if ((it = named_arguments.find("modulus")) != named_arguments.end()) { modulus = atoi(it->second); }
#endif
	}

	unsigned short max_dimension = std::numeric_limits<unsigned short>::max();
	unsigned short min_dimension = 0;
	coefficient_t modulus = 2;
	bool split_into_connected_components = false;
	bool directed = false;
	bool approximate_computation = false;
	size_t max_entries = std::numeric_limits<size_t>::max();
	std::string input_format = "flagser";
	std::string output_name = "";
	std::string output_format = "barcode";
	std::string cache = "";
	std::string hdf5_type = "";
	std::unique_ptr<filtration_algorithm_t> filtration_algorithm;

private:
	/* data */
	bool has_zero_filtration_and_no_explicit_output(const named_arguments_t& named_arguments) {
		return strlen(get_argument_or_default(named_arguments, "out-format", "")) == 0 &&
		       std::string(get_argument_or_default(named_arguments, "filtration", "zero")) == "zero";
	}
};

#endif /* ifndef PARAMETERS_H */
