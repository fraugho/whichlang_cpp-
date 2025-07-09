#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include "language_detector3.hpp"

// Hash map for O(1) lookup instead of O(n) array search
const std::unordered_map<std::string, Lang> codeToLangMap = {
    {"af", Lang::Afr},   // Afrikaans
    {"ar", Lang::Ara},   // Arabic
    {"az", Lang::Aze},   // Azerbaijani
    {"be", Lang::Bel},   // Belarusian
    {"bn", Lang::Ben},   // Bengali
    {"bs", Lang::Bos},   // Bosnian
    {"bg", Lang::Bul},   // Bulgarian
    {"ca", Lang::Cat},   // Catalan
    {"cs", Lang::Ces},   // Czech
    {"zh", Lang::Cmn},   // Mandarin Chinese
    {"cy", Lang::Cym},   // Welsh
    {"da", Lang::Dan},   // Danish
    {"de", Lang::Deu},   // German
    {"el", Lang::Ell},   // Modern Greek
    {"en", Lang::Eng},   // English
    {"eo", Lang::Epo},   // Esperanto
    {"et", Lang::Est},   // Estonian
    {"eu", Lang::Eus},   // Basque
    {"fi", Lang::Fin},   // Finnish
    {"fr", Lang::Fra},   // French
    {"ga", Lang::Gle},   // Irish
    {"gu", Lang::Guj},   // Gujarati
    {"he", Lang::Heb},   // Hebrew
    {"hi", Lang::Hin},   // Hindi
    {"hr", Lang::Hrv},   // Croatian
    {"hu", Lang::Hun},   // Hungarian
    {"hy", Lang::Hye},   // Armenian
    {"id", Lang::Ind},   // Indonesian
    {"is", Lang::Isl},   // Icelandic
    {"it", Lang::Ita},   // Italian
    {"ja", Lang::Jpn},   // Japanese
    {"ka", Lang::Kat},   // Georgian
    {"kk", Lang::Kaz},   // Kazakh
    {"ko", Lang::Kor},   // Korean
    {"la", Lang::Lat},   // Latin
    {"lt", Lang::Lit},   // Lithuanian
    {"lg", Lang::Lug},   // Ganda
    {"lv", Lang::Lvs},   // Standard Latvian
    {"mr", Lang::Mar},   // Marathi
    {"mk", Lang::Mkd},   // Macedonian
    {"mn", Lang::Mon},   // Mongolian
    {"mi", Lang::Mri},   // Maori
    {"nl", Lang::Nld},   // Dutch
    {"nn", Lang::Nno},   // Norwegian Nynorsk
    {"nb", Lang::Nob},   // Norwegian Bokmål
    {"pa", Lang::Pan},   // Panjabi
    {"fa", Lang::Pes},   // Iranian Persian
    {"pl", Lang::Pol},   // Polish
    {"pt", Lang::Por},   // Portuguese
    {"ro", Lang::Ron},   // Romanian
    {"ru", Lang::Rus},   // Russian
    {"sk", Lang::Slk},   // Slovak
    {"sl", Lang::Slv},   // Slovenian
    {"sn", Lang::Sna},   // Shona
    {"so", Lang::Som},   // Somali
    {"st", Lang::Sot},   // Southern Sotho
    {"es", Lang::Spa},   // Spanish
    {"sq", Lang::Sqi},   // Albanian
    {"sr", Lang::Srp},   // Serbian
    {"sv", Lang::Swe},   // Swedish
    {"sw", Lang::Swh},   // Swahili
    {"ta", Lang::Tam},   // Tamil
    {"te", Lang::Tel},   // Telugu
    {"tl", Lang::Tgl},   // Tagalog
    {"th", Lang::Tha},   // Thai
    {"tn", Lang::Tsn},   // Tswana
    {"ts", Lang::Tso},   // Tsonga
    {"tr", Lang::Tur},   // Turkish
    {"uk", Lang::Ukr},   // Ukrainian
    {"ur", Lang::Urd},   // Urdu
    {"vi", Lang::Vie},   // Vietnamese
    {"xh", Lang::Xho},   // Xhosa
    {"yo", Lang::Yor},   // Yoruba
    {"ms", Lang::Zsm},   // Standard Malay
    {"zu", Lang::Zul},   // Zulu
};

Lang codeToLang(const std::string& code) {
    auto it = codeToLangMap.find(code);
    if (it != codeToLangMap.end()) {
        return it->second;
    }
    return Lang::Eng; // default
}

std::vector<std::string> readLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "ERROR: Cannot open " << filename << std::endl;
        return lines;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    
    return lines;
}

// Helper function to convert Lang enum back to string code
std::string langToCode(Lang lang) {
    for (const auto& pair : codeToLangMap) {
        if (pair.second == lang) {
            return pair.first;
        }
    }
    return "unknown";
}

int main() {
    std::vector<std::string> labels = readLines("../data/labels.txt");
    std::vector<std::string> texts = readLines("../data/text.txt");
    
    if (labels.empty() || texts.empty() || labels.size() != texts.size()) {
        std::cerr << "Error loading files or size mismatch" << std::endl;
        return 1;
    }
    
    int total = 0, correct = 0, unknown = 0;
    std::unordered_map<std::string, int> errorsByLanguage;
    std::unordered_map<std::string, int> totalByLanguage;
    
    // Open file to write incorrect predictions
    std::ofstream errorFile("errors.txt");
    if (!errorFile.is_open()) {
        std::cerr << "Warning: Cannot create errors.txt file" << std::endl;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < labels.size(); ++i) {
        Lang expected = codeToLang(labels[i]);
        Lang detected = LanguageDetector::detectLanguage(texts[i]);
        
        // Count total samples per language
        totalByLanguage[labels[i]]++;
        
        if (expected == Lang::Eng && labels[i] != "en") {
            unknown++;
        } else if (detected == expected) {
            correct++;
        } else {
            // Count errors per language
            errorsByLanguage[labels[i]]++;
            
            // Write incorrect prediction to file
            if (errorFile.is_open()) {
                errorFile << "Expected: " << labels[i] << " (" << langToCode(expected) << ")" << std::endl;
                errorFile << "Got: " << langToCode(detected) << std::endl;
                errorFile << "Text: " << texts[i] << std::endl;
                errorFile << "---" << std::endl;
            }
        }
        
        total++;
    }
    
    errorFile.close();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double accuracy = (double)correct / total * 100.0;
    double totalTime = duration.count() / 1000.0;
    double avgTime = totalTime / total;
    
    std::cout << "Samples: " << total << std::endl;
    std::cout << "Correct: " << correct << std::endl;
    std::cout << "Accuracy: " << std::fixed << std::setprecision(2) << accuracy << "%" << std::endl;
    std::cout << "Total time: " << std::fixed << std::setprecision(2) << totalTime << " ms" << std::endl;
    std::cout << "Avg time: " << std::fixed << std::setprecision(4) << avgTime << " ms/sample" << std::endl;
    std::cout << "Errors written to: errors.txt" << std::endl;
    
    // Calculate and display error statistics by language
    std::cout << "\nError breakdown by language:" << std::endl;
    std::cout << std::setw(8) << "Lang" << std::setw(8) << "Errors" << std::setw(8) << "Total" << std::setw(10) << "Error%" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    int totalErrors = total - correct - unknown;
    
    // Create vector of pairs for sorting
    std::vector<std::pair<std::string, int>> errorPairs(errorsByLanguage.begin(), errorsByLanguage.end());
    
    // Sort by number of errors (descending)
    std::sort(errorPairs.begin(), errorPairs.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& pair : errorPairs) {
        const std::string& lang = pair.first;
        int errors = pair.second;
        int totalSamples = totalByLanguage[lang];
        double errorPercent = (double)errors / totalSamples * 100.0;
        double errorShare = (double)errors / totalErrors * 100.0;
        
        std::cout << std::setw(8) << lang 
                  << std::setw(8) << errors 
                  << std::setw(8) << totalSamples 
                  << std::setw(9) << std::fixed << std::setprecision(1) << errorPercent << "%" 
                  << " (" << std::setprecision(1) << errorShare << "% of all errors)" << std::endl;
    }
    
    return 0;
}
