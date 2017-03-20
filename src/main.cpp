/* longlines -- find long lines in text files
   Copyright (C) 2017 Gregory Kikola.

   This file is part of longlines.

   longlines is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   longlines is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with longlines.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Written by Gregory Kikola <gkikola@gmail.com>. */

#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include "option.hpp"
#include "version.h"

constexpr unsigned int default_length = 80u;

  const std::string usage = "\
Usage: longlines [OPTION...] FILES\n\
Read each text file in FILES and display any lines that exceed the\n\
length specified by the --length option (the default length is 80\n\
characters).\n\
";

  const std::string version = "\
longlines " LONGLINES_VERSION "\n\
Copyright (C) 2017 Gregory Kikola\n\
License GPLv3+: GNU GPL version 3 or later\n\
<http://www.gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n\
\n\
Written by Gregory Kikola <gkikola@gmail.com>.\n\
";

struct Settings {
  bool all = false;
  bool filenames = true;
  bool line_numbers = true;
  unsigned int length = default_length;
  unsigned int out_length = default_length;
};

bool process_options(const OptionParser& op, Settings& settings);
void process_file(std::ostream& out, std::istream& in,
                  const std::string& filename, const Settings& settings);

int main(int argc, char* argv[])
{
  try {
    OptionParser op = {
      {'a', "all", "", "show whole lines instead of just the beginning "
       "of each"},
      {'f', "filenames", "", "suppresses filenames"},
      {'l', "length", "MAXLENGTH", "display only lines with length exceeding "
       "MAXLENGTH characters (default 80)"},
      {'L', "output-length", "MAXLENGTH", "do not display more than "
       "MAXLENGTH characters on each line of output (default 80)"},
      {'n', "line-numbers", "", "suppresses line numbers"},
      {'?', "help", "", "display detailed usage information and then exit"},
      {'u', "usage", "", "display a short usage message and then exit"},
      {0, "version", "", "display program version and exit"}
    };
  
    op.parse(argc, argv);

    Settings settings;
    if (process_options(op, settings))
      return 0; //exit on --version or --help

    if (op.program_args().empty())
      std::cout << usage;
    
    for (const auto& s : op.program_args()) {
      if (s == "-")
        process_file(std::cout, std::cin, "", settings);
      else {
        std::ifstream ifs(s);
        if (ifs.is_open())
          process_file(std::cout, ifs, s, settings);
        else
          throw std::runtime_error("could not open file \"" + s + "\"");
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;

    return 1;
  }

  return 0;
}

bool process_options(const OptionParser& op, Settings& settings)
{  
  for (const auto& o : op) {
    if (o.long_name == "version") {
      std::cout << version;
      return true;
    }
    else {
      int arg;
      switch (o.short_name) {
      case 'a':
        settings.all = true;
        break;
      case 'f':
        settings.filenames = false;
        break;
      case 'L':
      case 'l':
        try {
          arg = std::stoi(o.argument);
        } catch (std::invalid_argument) {
          throw std::runtime_error("length must be a number");
        } catch (std::out_of_range) {
          throw std::runtime_error("length is out of range");
        }

        if (arg < 0)
          throw std::runtime_error("length cannot be negative");
        else {
          if (o.short_name == 'l')
            settings.length = static_cast<unsigned int>(arg);
          else if (o.short_name == 'L')
            settings.out_length = static_cast<unsigned int>(arg);
        }
        break;
      case 'n':
        settings.line_numbers = false;
        break;
      case 'u':
        std::cout << usage;
        return true;
      case '?':
        std::cout << usage << std::endl;
        op.print_usage(std::cout);
        return true;
      }
    }
  }

  return false;
}

void process_file(std::ostream& out, std::istream& in,
                  const std::string& filename, const Settings& settings)
{
  std::string line, out_line;
  std::size_t line_no = 1;
  unsigned int cur_length;
  
  while (getline(in, line)) {
    if (line.length() > settings.length) {
      out_line = "";
      
      if (settings.filenames)
        out_line += filename;
      
      if (settings.line_numbers) {
        if (settings.filenames && !filename.empty())
          out_line += "(";
        
        out_line += std::to_string(line_no);

        if (settings.filenames && !filename.empty())
          out_line += ")";
      }

      if ((settings.filenames && !filename.empty()) || settings.line_numbers)
        out_line += ": ";

      out_line += line;

      if (settings.all || out_line.length() <= settings.out_length)
        out << out_line << "\n";
      else        
        out << out_line.substr(0, settings.out_length) << "\n";
    }

    ++line_no;
  }

  out << std::flush;
}
