//
// Created by Edge on 2021/1/8.
//

#ifndef RAY_SHADERINCLUDE_H
#define RAY_SHADERINCLUDE_H

#include <string>
#include <fstream>
#include <iostream>

class ShaderInclude {
public:

    struct ShaderSource {
        std::string source;
        std::string path;
    };

    static ShaderSource load(const std::string &sourceFilename, const std::string &includeIdentifier = "#include") {
        std::string fullSourceCode = "";
        std::ifstream sourceFile(sourceFilename);

        if(!sourceFile.is_open()) {
            std::cerr << "Could not open the shader at location " << sourceFilename << std::endl;
            exit(1);
        }

        std::string lineBuffer;
        while(std::getline(sourceFile, lineBuffer)) {
            if(lineBuffer.find(includeIdentifier) != lineBuffer.npos) {
                // Remove include identifier, this will cause the include source remain
                lineBuffer.erase(0, includeIdentifier.size());
                lineBuffer.erase(0, lineBuffer.find_first_not_of(" "));

                std::string pathOfIncludeFile;
                // Get shader file relative to current shader file path
                getFilePath(sourceFilename, pathOfIncludeFile);
                lineBuffer.insert(0, pathOfIncludeFile);

                // By using recursion, the new include file can be extracted and inserted at this location in the shader source code
                loadRecursive(lineBuffer, fullSourceCode);

                continue;
            }
            fullSourceCode += lineBuffer + '\n';
        }
        fullSourceCode += '\0';

        sourceFile.close();
        return ShaderSource{fullSourceCode, sourceFilename};
    }

    static void loadRecursive(const std::string &currentSourceFilename, std::string &source, const std::string &includeIdentifier = "#include") {
        std::ifstream sourceFile(currentSourceFilename);

        if(!sourceFile.is_open()) {
            std::cerr << "Could not open the shader at location " << currentSourceFilename << std::endl;
            exit(1);
        }

        std::string lineBuffer;
        while(std::getline(sourceFile, lineBuffer)) {
            if(lineBuffer.find(includeIdentifier) != lineBuffer.npos) {
                // Remove include identifier, this will cause the include source remain
                lineBuffer.erase(0, includeIdentifier.size());
                lineBuffer.erase(0, lineBuffer.find_first_not_of(" "));

                std::string pathOfIncludeFile;
                // Get shader file relative to current shader file path
                getFilePath(currentSourceFilename, pathOfIncludeFile);
                lineBuffer.insert(0, pathOfIncludeFile);

                // By using recursion, the new include file can be extracted and inserted at this location in the shader source code
                loadRecursive(lineBuffer, source);

                continue;
            }
            source += lineBuffer + "\n";
        }

        sourceFile.close();
    }
private:
    static void getFilePath(const std::string& fullPath, std::string& pathWithoutFileName)
    {
        // Remove the file name and store the path to this folder
        size_t found = fullPath.find_last_of("/\\");
        pathWithoutFileName = fullPath.substr(0, found + 1);
    }
};

#endif //RAY_SHADERINCLUDE_H
