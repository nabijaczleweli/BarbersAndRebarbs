// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "app/application.hpp"
#include "reference/container.hpp"
#include "resource/localizer.hpp"
#include "util/broken_gcc.hpp"
#include "util/configurable.hpp"
#include "util/file.hpp"
#include <cpponfiguration/cpponfig_version.hpp>
#include <audiere.h>
#include <ctime>
#include <iostream>
#include <random>
#include <stdexcept>


using namespace std;
using namespace cpponfig;


static application * app;


void init_app(int argc, char * argv[]);
void deinit_app();
void init_deps();


int main(int argc, char * argv[]) {
	init_deps();
	init_app(argc, argv);
	const auto result = app->run();
	deinit_app();
	if(result)
		cerr << "`app->run()` failed! Oh noes!";
	return result;
}


void init_app(int, char *[]) {
	const_cast<localizer &>(fallback_izer).open();
	const_cast<localizer &>(local_izer).open(app_language);
	const_cast<localizer &>(global_izer).merge(local_izer).merge(fallback_izer);

	if(!app)
		app = new application;
	else
		throw invalid_argument("`app` has been tampered with (`!= nullptr`)! Value: " + to_string<void *>(app));

	app_configuration.configure();
}

void deinit_app() {
	delete app;
	app = nullptr;
}

void init_deps() {
	class deps_configable : public configurable {
		private:
			virtual void config(configuration & cfg) override {
				cout << "GCC version " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__ << " initializing...\n";
				srand(time(nullptr));
				cout << "GCC initialized.\n"

				        "SFML version " << SFML_VERSION_MAJOR << '.' << SFML_VERSION_MINOR << " doesn\'t need initialization.\n"
				        "audiere version " << audiere::GetVersion() << " doesn\'t need initialization.\n"
				        "Cpp-NBT doesn\'t need initialization.\n";

				if(cfg.contains("system:language"))
					const_cast<string &>(app_language) = cfg.get("system:language").textual();
				else {
					string files;
					for(const auto & name : list_files(localization_root))
						if(name[0] != '.' && name.size() == 10 && name.find(".lang") == 5)
							files += name.substr(0, name.find(".lang")) + ", ";
					cfg.get("system:language", property(app_language, "Available languages: " + files.substr(0, files.size() - 2)));
				}
			}

		public:
			deps_configable() : configurable(nothrow) {}

			void preconfig() {
				cout << "Cpponfiguration version " << cpponfiguration_version << " initializing...\n";
				configuration::datetime_footer_type = configuration::datetime_mode::gmt;
				cout << "Cpponfiguration initialized.\n";
			}
	};


	cout << "Initializing dependencies under "
#ifdef _WIN32
	"Windows"
#elif defined(unix) || defined(__unix__) || defined(__unix)
	"UNIX"
#elif defined(__APPLE__)
	"Mac OS X"
#elif defined(__linux__)
	"Linux"
#elif defined(__FreeBSD__)
	"FreeBSD"
#else
	"an unknown OS"
#endif
	"...\n\n";


	deps_configable dependencies_config;
	dependencies_config.preconfig();
	app_configuration.load();
	app_configuration.add(dependencies_config);
	app_configuration.configure();
	app_configuration.sof_comments = {"This is " + app_name + "\'s configuration file."};
	cout << "\nAll dependencies initialized.\n\n";
}
