//
//  main.cpp
//  basic-vamp-host
//
//  Created by Lucas Thompson on 13/05/2016.
//  Copyright © 2016 LTDSP. All rights reserved.
//

#include "vamp-simple-host.cpp"

void usage(const char *name)
{
    cerr << "\n"
    << name << ": A command-line host for Vamp audio analysis plugins.\n\n"
    "Centre for Digital Music, Queen Mary, University of London.\n"
    "Copyright 2006-2009 Chris Cannam and QMUL.\n"
    "Freely redistributable; published under a BSD-style license.\n\n"
    "Usage:\n\n"
    "  " << name << " [-s] pluginlibrary[." << PLUGIN_SUFFIX << "]:plugin[:output] file.wav [-o out.txt]\n"
    "  " << name << " [-s] pluginlibrary[." << PLUGIN_SUFFIX << "]:plugin file.wav [outputno] [-o out.txt]\n\n"
    "    -- Load plugin id \"plugin\" from \"pluginlibrary\" and run it on the\n"
    "       audio data in \"file.wav\", retrieving the named \"output\", or output\n"
    "       number \"outputno\" (the first output by default) and dumping it to\n"
    "       standard output, or to \"out.txt\" if the -o option is given.\n\n"
    "       \"pluginlibrary\" should be a library name, not a file path; the\n"
    "       standard Vamp library search path will be used to locate it.  If\n"
    "       a file path is supplied, the directory part(s) will be ignored.\n\n"
    "       If the -s option is given, results will be labelled with the audio\n"
    "       sample frame at which they occur. Otherwise, they will be labelled\n"
    "       with time in seconds.\n\n"
    "  " << name << " -l\n"
    "  " << name << " --list\n\n"
    "    -- List the plugin libraries and Vamp plugins in the library search path\n"
    "       in a verbose human-readable format.\n\n"
    "  " << name << " -L\n"
    "  " << name << " --list-full\n\n"
    "    -- List all data reported by all the Vamp plugins in the library search\n"
    "       path in a very verbose human-readable format.\n\n"
    "  " << name << " --list-ids\n\n"
    "    -- List the plugins in the search path in a terse machine-readable format,\n"
    "       in the form vamp:soname:identifier.\n\n"
    "  " << name << " --list-outputs\n\n"
    "    -- List the outputs for plugins in the search path in a machine-readable\n"
    "       format, in the form vamp:soname:identifier:output.\n\n"
    "  " << name << " --list-by-category\n\n"
    "    -- List the plugins as a plugin index by category, in a machine-readable\n"
    "       format.  The format may change in future releases.\n\n"
    "  " << name << " -p\n\n"
    "    -- Print out the Vamp library search path.\n\n"
    "  " << name << " -v\n\n"
    "    -- Display version information only.\n"
    << endl;
    exit(2);
}

int main(int argc, char **argv)
{
    char *scooter = argv[0];
    char *name = 0;
    while (scooter && *scooter) {
        if (*scooter == '/' || *scooter == '\\') name = ++scooter;
        else ++scooter;
    }
    if (!name || !*name) name = argv[0];
    
    if (argc < 2) usage(name);
    
    if (argc == 2) {
        
        if (!strcmp(argv[1], "-v")) {
            
            cout << "Simple Vamp plugin host version: " << HOST_VERSION << endl
            << "Vamp API version: " << VAMP_API_VERSION << endl
            << "Vamp SDK version: " << VAMP_SDK_VERSION << endl;
            return 0;
            
        } else if (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--list")) {
            
            printPluginPath(true);
            enumeratePlugins(PluginInformation);
            return 0;
            
        } else if (!strcmp(argv[1], "-L") || !strcmp(argv[1], "--list-full")) {
            
            enumeratePlugins(PluginInformationDetailed);
            return 0;
            
        } else if (!strcmp(argv[1], "-p")) {
            
            printPluginPath(false);
            return 0;
            
        } else if (!strcmp(argv[1], "--list-ids")) {
            
            enumeratePlugins(PluginIds);
            return 0;
            
        } else if (!strcmp(argv[1], "--list-outputs")) {
            
            enumeratePlugins(PluginOutputIds);
            return 0;
            
        } else if (!strcmp(argv[1], "--list-by-category")) {
            
            printPluginCategoryList();
            return 0;
            
        } else usage(name);
    }
    
    if (argc < 3) usage(name);
    
    bool useFrames = false;
    
    int base = 1;
    if (!strcmp(argv[1], "-s")) {
        useFrames = true;
        base = 2;
    }
    
    string soname = argv[base];
    string wavname = argv[base+1];
    string plugid = "";
    string output = "";
    int outputNo = -1;
    string outfilename;
    
    if (argc >= base+3) {
        
        int idx = base+2;
        
        if (isdigit(*argv[idx])) {
            outputNo = atoi(argv[idx++]);
        }
        
        if (argc == idx + 2) {
            if (!strcmp(argv[idx], "-o")) {
                outfilename = argv[idx+1];
            } else usage(name);
        } else if (argc != idx) {
            (usage(name));
        }
    }
    
    cerr << endl << name << ": Running..." << endl;
    
    cerr << "Reading file: \"" << wavname << "\", writing to ";
    if (outfilename == "") {
        cerr << "standard output" << endl;
    } else {
        cerr << "\"" << outfilename << "\"" << endl;
    }
    
    string::size_type sep = soname.find(':');
    
    if (sep != string::npos) {
        plugid = soname.substr(sep + 1);
        soname = soname.substr(0, sep);
        
        sep = plugid.find(':');
        if (sep != string::npos) {
            output = plugid.substr(sep + 1);
            plugid = plugid.substr(0, sep);
        }
    }
    
    if (plugid == "") {
        usage(name);
    }
    
    if (output != "" && outputNo != -1) {
        usage(name);
    }
    
    if (output == "" && outputNo == -1) {
        outputNo = 0;
    }
    
    return runPlugin(name, soname, plugid, output, outputNo,
                     wavname, outfilename, useFrames);
}