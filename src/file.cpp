#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include "language_detector.hpp"

// Hash map for O(1) lookup instead of O(n) array search
const std::unordered_map<std::string, Lang> codeToLangMap = {
    {"ab", Lang::Abk},   // Abkhazian
    {"acm", Lang::Acm},  // Mesopotamian Arabic
    {"ady", Lang::Ady},  // Adyghe
    {"afb", Lang::Afb},  // Gulf Arabic
    {"afh", Lang::Afh},  // Afrihili
    {"af", Lang::Afr},   // Afrikaans
    {"aii", Lang::Aii},  // Assyrian Neo-Aramaic
    {"ain", Lang::Ain},  // Ainu (Japan)
    {"ajp", Lang::Ajp},  // South Levantine Arabic
    {"akl", Lang::Akl},  // Aklanon
    {"aln", Lang::Aln},  // Gheg Albanian
    {"alt", Lang::Alt},  // Southern Altai
    {"am", Lang::Amh},   // Amharic
    {"ang", Lang::Ang},  // Old English
    {"aoz", Lang::Aoz},  // Uab Meto
    {"apc", Lang::Apc},  // North Levantine Arabic
    {"ar", Lang::Ara},   // Arabic
    {"an", Lang::Arg},   // Aragonese
    {"arq", Lang::Arq},  // Algerian Arabic
    {"ary", Lang::Ary},  // Moroccan Arabic
    {"arz", Lang::Arz},  // Egyptian Arabic
    {"as", Lang::Asm},   // Assamese
    {"ast", Lang::Ast},  // Asturian
    {"avk", Lang::Avk},  // Kotava
    {"awa", Lang::Awa},  // Awadhi
    {"ayl", Lang::Ayl},  // Libyan Arabic
    {"ay", Lang::Aym},   // Aymara
    {"az", Lang::Aze},   // Azerbaijani
    {"ba", Lang::Bak},   // Bashkir
    {"bal", Lang::Bal},  // Baluchi
    {"bm", Lang::Bam},   // Bambara
    {"ban", Lang::Ban},  // Balinese
    {"bar", Lang::Bar},  // Bavarian
    {"bcl", Lang::Bcl},  // Central Bikol
    {"be", Lang::Bel},   // Belarusian
    {"bn", Lang::Ben},   // Bengali
    {"ber", Lang::Ber},  // Berber languages
    {"bfz", Lang::Bfz},  // Mahasu Pahari
    {"bho", Lang::Bho},  // Bhojpuri
    {"bi", Lang::Bis},   // Bislama
    {"bjn", Lang::Bjn},  // Banjar
    {"bo", Lang::Bod},   // Tibetan
    {"bom", Lang::Bom},  // Berom
    {"bs", Lang::Bos},   // Bosnian
    {"br", Lang::Bre},   // Breton
    {"brx", Lang::Brx},  // Bodo (India)
    {"bua", Lang::Bua},  // Buriat
    {"bg", Lang::Bul},   // Bulgarian
    {"bvy", Lang::Bvy},  // Baybayanon
    {"bzt", Lang::Bzt},  // Brithenig
    {"ca", Lang::Cat},   // Catalan
    {"cay", Lang::Cay},  // Cayuga
    {"cbk", Lang::Cbk},  // Chavacano
    {"ceb", Lang::Ceb},  // Cebuano
    {"cs", Lang::Ces},   // Czech
    {"ch", Lang::Cha},   // Chamorro
    {"ce", Lang::Che},   // Chechen
    {"chg", Lang::Chg},  // Chagatai
    {"chn", Lang::Chn},  // Chinook jargon
    {"cho", Lang::Cho},  // Choctaw
    {"chr", Lang::Chr},  // Cherokee
    {"cv", Lang::Chv},   // Chuvash
    {"cjy", Lang::Cjy},  // Jinyu Chinese
    {"ckb", Lang::Ckb},  // Central Kurdish
    {"ckt", Lang::Ckt},  // Chukot
    {"zh", Lang::Cmn},   // Mandarin Chinese
    {"cmo", Lang::Cmo},  // Central Mnong
    {"kw", Lang::Cor},   // Cornish
    {"co", Lang::Cos},   // Corsican
    {"cpi", Lang::Cpi},  // Chinese Pidgin English
    {"crh", Lang::Crh},  // Crimean Tatar
    {"crk", Lang::Crk},  // Plains Cree
    {"crs", Lang::Crs},  // Seselwa Creole French
    {"csb", Lang::Csb},  // Kashubian
    {"cycl", Lang::Cycl}, // CycL
    {"cy", Lang::Cym},   // Welsh
    {"cyo", Lang::Cyo},  // Cuyonon
    {"da", Lang::Dan},   // Danish
    {"de", Lang::Deu},   // German
    {"diq", Lang::Diq},  // Dimli
    {"dv", Lang::Div},   // Dhivehi
    {"dng", Lang::Dng},  // Dungan
    {"drt", Lang::Drt},  // Drents
    {"dsb", Lang::Dsb},  // Lower Sorbian
    {"dtp", Lang::Dtp},  // Kadazan Dusun
    {"dws", Lang::Dws},  // Dutton World Speedwords
    {"egl", Lang::Egl},  // Emilian
    {"el", Lang::Ell},   // Modern Greek
    {"emx", Lang::Emx},  // Erromintxela
    {"en", Lang::Eng},   // English
    {"enm", Lang::Enm},  // Middle English
    {"eo", Lang::Epo},   // Esperanto
    {"et", Lang::Est},   // Estonian
    {"eu", Lang::Eus},   // Basque
    {"evn", Lang::Evn},  // Evenki
    {"ee", Lang::Ewe},   // Ewe
    {"ext", Lang::Ext},  // Extremaduran
    {"fo", Lang::Fao},   // Faroese
    {"fj", Lang::Fij},   // Fijian
    {"fi", Lang::Fin},   // Finnish
    {"fkv", Lang::Fkv},  // Kven Finnish
    {"fr", Lang::Fra},   // French
    {"frm", Lang::Frm},  // Middle French
    {"fro", Lang::Fro},  // Old French
    {"frr", Lang::Frr},  // Northern Frisian
    {"fy", Lang::Fry},   // Western Frisian
    {"fuc", Lang::Fuc},  // Pulaar
    {"fur", Lang::Fur},  // Friulian
    {"fuv", Lang::Fuv},  // Nigerian Fulfulde
    {"gaa", Lang::Gaa},  // Ga
    {"gag", Lang::Gag},  // Gagauz
    {"gan", Lang::Gan},  // Gan Chinese
    {"gbm", Lang::Gbm},  // Garhwali
    {"gcf", Lang::Gcf},  // Guadeloupean Creole French
    {"gil", Lang::Gil},  // Gilbertese
    {"gd", Lang::Gla},   // Scottish Gaelic
    {"ga", Lang::Gle},   // Irish
    {"gl", Lang::Glg},   // Galician
    {"gv", Lang::Glv},   // Manx
    {"gom", Lang::Gom},  // Goan Konkani
    {"gos", Lang::Gos},  // Gronings
    {"got", Lang::Got},  // Gothic
    {"grc", Lang::Grc},  // Ancient Greek
    {"gn", Lang::Grn},   // Guarani
    {"gsw", Lang::Gsw},  // Swiss German
    {"guc", Lang::Guc},  // Wayuu
    {"gu", Lang::Guj},   // Gujarati
    {"hak", Lang::Hak},  // Hakka Chinese
    {"ht", Lang::Hat},   // Haitian
    {"ha", Lang::Hau},   // Hausa
    {"haw", Lang::Haw},  // Hawaiian
    {"hax", Lang::Hax},  // Southern Haida
    {"hbo", Lang::Hbo},  // Ancient Hebrew
    {"hdn", Lang::Hdn},  // Northern Haida
    {"he", Lang::Heb},   // Hebrew
    {"hif", Lang::Hif},  // Fiji Hindi
    {"hil", Lang::Hil},  // Hiligaynon
    {"hi", Lang::Hin},   // Hindi
    {"hnj", Lang::Hnj},  // Hmong Njua
    {"hoc", Lang::Hoc},  // Ho
    {"hr", Lang::Hrv},   // Croatian
    {"hrx", Lang::Hrx},  // Hunsrik
    {"hsb", Lang::Hsb},  // Upper Sorbian
    {"hsn", Lang::Hsn},  // Xiang Chinese
    {"hu", Lang::Hun},   // Hungarian
    {"hy", Lang::Hye},   // Armenian
    {"iba", Lang::Iba},  // Iban
    {"ig", Lang::Ibo},   // Igbo
    {"io", Lang::Ido},   // Ido
    {"igs", Lang::Igs},  // Interglossa
    {"ii", Lang::Iii},   // Sichuan Yi
    {"ike", Lang::Ike},  // Eastern Canadian Inuktitut
    {"ile", Lang::Ile},  // Interlingue
    {"ilo", Lang::Ilo},  // Iloko
    {"ia", Lang::Ina},   // Interlingua
    {"id", Lang::Ind},   // Indonesian
    {"is", Lang::Isl},   // Icelandic
    {"it", Lang::Ita},   // Italian
    {"izh", Lang::Izh},  // Ingrian
    {"jam", Lang::Jam},  // Jamaican Creole English
    {"jv", Lang::Jav},   // Javanese
    {"jbo", Lang::Jbo},  // Lojban
    {"jdt", Lang::Jdt},  // Judeo-Tat
    {"jpa", Lang::Jpa},  // Jewish Palestinian Aramaic
    {"ja", Lang::Jpn},   // Japanese
    {"kaa", Lang::Kaa},  // Kara-Kalpak
    {"kab", Lang::Kab},  // Kabyle
    {"kl", Lang::Kal},   // Kalaallisut
    {"kam", Lang::Kam},  // Kamba (Kenya)
    {"kn", Lang::Kan},   // Kannada
    {"ks", Lang::Kas},   // Kashmiri
    {"ka", Lang::Kat},   // Georgian
    {"kk", Lang::Kaz},   // Kazakh
    {"kek", Lang::Kek},  // Kekchí
    {"kha", Lang::Kha},  // Khasi
    {"km", Lang::Khm},   // Central Khmer
    {"rw", Lang::Kin},   // Kinyarwanda
    {"ky", Lang::Kir},   // Kirghiz
    {"kiu", Lang::Kiu},  // Kirmanjki
    {"kjh", Lang::Kjh},  // Khakas
    {"klj", Lang::Klj},  // Turkic Khalaj
    {"kmr", Lang::Kmr},  // Northern Kurdish
    {"koi", Lang::Koi},  // Komi-Permyak
    {"ko", Lang::Kor},   // Korean
    {"kpv", Lang::Kpv},  // Komi-Zyrian
    {"krc", Lang::Krc},  // Karachay-Balkar
    {"krl", Lang::Krl},  // Karelian
    {"ksh", Lang::Ksh},  // Kölsch
    {"kum", Lang::Kum},  // Kumyk
    {"kxi", Lang::Kxi},  // Keningau Murut
    {"kzj", Lang::Kzj},  // Coastal Kadazan
    {"laa", Lang::Laa},  // Southern Subanen
    {"lad", Lang::Lad},  // Ladino
    {"lo", Lang::Lao},   // Lao
    {"la", Lang::Lat},   // Latin
    {"ldn", Lang::Ldn},  // Láadan
    {"lfn", Lang::Lfn},  // Lingua Franca Nova
    {"lij", Lang::Lij},  // Ligurian
    {"li", Lang::Lim},   // Limburgan
    {"ln", Lang::Lin},   // Lingala
    {"lt", Lang::Lit},   // Lithuanian
    {"liv", Lang::Liv},  // Liv
    {"lkt", Lang::Lkt},  // Lakota
    {"lld", Lang::Lld},  // Ladin
    {"lmo", Lang::Lmo},  // Lombard
    {"lou", Lang::Lou},  // Louisiana Creole
    {"ltg", Lang::Ltg},  // Latgalian
    {"lb", Lang::Ltz},   // Luxembourgish
    {"lg", Lang::Lug},   // Ganda
    {"lut", Lang::Lut},  // Lushootseed
    {"lv", Lang::Lvs},   // Standard Latvian
    {"lzh", Lang::Lzh},  // Literary Chinese
    {"lzz", Lang::Lzz},  // Laz
    {"mad", Lang::Mad},  // Madurese
    {"mh", Lang::Mah},   // Marshallese
    {"mai", Lang::Mai},  // Maithili
    {"ml", Lang::Mal},   // Malayalam
    {"mr", Lang::Mar},   // Marathi
    {"max", Lang::Max},  // North Moluccan Malay
    {"mdf", Lang::Mdf},  // Moksha
    {"mfe", Lang::Mfe},  // Morisyen
    {"mgm", Lang::Mgm},  // Mambae
    {"mhr", Lang::Mhr},  // Eastern Mari
    {"mic", Lang::Mic},  // Mi'kmaq
    {"mik", Lang::Mik},  // Mikasuki
    {"min", Lang::Min},  // Minangkabau
    {"mk", Lang::Mkd},   // Macedonian
    {"mg", Lang::Mlg},   // Malagasy
    {"mt", Lang::Mlt},   // Maltese
    {"mnc", Lang::Mnc},  // Manchu
    {"mni", Lang::Mni},  // Manipuri
    {"mnr", Lang::Mnr},  // Mono (USA)
    {"mnw", Lang::Mnw},  // Mon
    {"moh", Lang::Moh},  // Mohawk
    {"mn", Lang::Mon},   // Mongolian
    {"mi", Lang::Mri},   // Maori
    {"mrj", Lang::Mrj},  // Western Mari
    {"mus", Lang::Mus},  // Creek
    {"mvv", Lang::Mvv},  // Tagal Murut
    {"mwl", Lang::Mwl},  // Mirandese
    {"mww", Lang::Mww},  // Hmong Daw
    {"my", Lang::Mya},   // Burmese
    {"myv", Lang::Myv},  // Erzya
    {"nah", Lang::Nah},  // Nahuatl
    {"na", Lang::Nau},   // Nauru
    {"nv", Lang::Nav},   // Navajo
    {"nch", Lang::Nch},  // Central Huasteca Nahuatl
    {"nds", Lang::Nds},  // Low German
    {"new", Lang::New},  // Newari
    {"ngt", Lang::Ngt},  // Ngeq
    {"ngu", Lang::Ngu},  // Guerrero Nahuatl
    {"niu", Lang::Niu},  // Niuean
    {"nl", Lang::Nld},   // Dutch
    {"nlv", Lang::Nlv},  // Orizaba Nahuatl
    {"nn", Lang::Nno},   // Norwegian Nynorsk
    {"nb", Lang::Nob},   // Norwegian Bokmål
    {"nog", Lang::Nog},  // Nogai
    {"non", Lang::Non},  // Old Norse
    {"nov", Lang::Nov},  // Novial
    {"ne", Lang::Npi},   // Nepali
    {"nst", Lang::Nst},  // Tase Naga
    {"nus", Lang::Nus},  // Nuer
    {"ny", Lang::Nya},   // Nyanja
    {"nys", Lang::Nys},  // Nyunga
    {"oar", Lang::Oar},  // Old Aramaic
    {"oc", Lang::Oci},   // Occitan
    {"ofs", Lang::Ofs},  // Old Frisian
    {"oj", Lang::Oji},   // Ojibwa
    {"ood", Lang::Ood},  // Tohono O'odham
    {"or", Lang::Ori},   // Oriya
    {"orv", Lang::Orv},  // Old Russian
    {"osp", Lang::Osp},  // Old Spanish
    {"os", Lang::Oss},   // Ossetian
    {"osx", Lang::Osx},  // Old Saxon
    {"ota", Lang::Ota},  // Ottoman Turkish
    {"otk", Lang::Otk},  // Old Turkish
    {"pag", Lang::Pag},  // Pangasinan
    {"pal", Lang::Pal},  // Pahlavi
    {"pam", Lang::Pam},  // Pampanga
    {"pa", Lang::Pan},   // Panjabi
    {"pap", Lang::Pap},  // Papiamento
    {"pau", Lang::Pau},  // Palauan
    {"pcd", Lang::Pcd},  // Picard
    {"pdc", Lang::Pdc},  // Pennsylvania German
    {"fa", Lang::Pes},   // Iranian Persian
    {"pfl", Lang::Pfl},  // Pfaelzisch
    {"phn", Lang::Phn},  // Phoenician
    {"pi", Lang::Pli},   // Pali
    {"pms", Lang::Pms},  // Piemontese
    {"pnb", Lang::Pnb},  // Western Panjabi
    {"pl", Lang::Pol},   // Polish
    {"pt", Lang::Por},   // Portuguese
    {"ppl", Lang::Ppl},  // Pipil
    {"prg", Lang::Prg},  // Prussian
    {"ps", Lang::Pus},   // Pushto
    {"quc", Lang::Quc},  // K'iche'
    {"qu", Lang::Que},   // Quechua
    {"qxq", Lang::Qxq},  // Qashqa'i
    {"qya", Lang::Qya},  // Quenya
    {"rap", Lang::Rap},  // Rapanui
    {"rel", Lang::Rel},  // Rendille
    {"rif", Lang::Rif},  // Tarifit
    {"rm", Lang::Roh},   // Romansh
    {"rom", Lang::Rom},  // Romany
    {"ro", Lang::Ron},   // Romanian
    {"rue", Lang::Rue},  // Rusyn
    {"rn", Lang::Run},   // Rundi
    {"ru", Lang::Rus},   // Russian
    {"ryu", Lang::Ryu},  // Central Okinawan
    {"sg", Lang::Sag},   // Sango
    {"sah", Lang::Sah},  // Yakut
    {"sa", Lang::San},   // Sanskrit
    {"sat", Lang::Sat},  // Santali
    {"scn", Lang::Scn},  // Sicilian
    {"sco", Lang::Sco},  // Scots
    {"sdh", Lang::Sdh},  // Southern Kurdish
    {"sgs", Lang::Sgs},  // Samogitian
    {"shi", Lang::Shi},  // Tachelhit
    {"shs", Lang::Shs},  // Shuswap
    {"shy", Lang::Shy},  // Tachawit
    {"si", Lang::Sin},   // Sinhala
    {"sjn", Lang::Sjn},  // Sindarin
    {"sk", Lang::Slk},   // Slovak
    {"sl", Lang::Slv},   // Slovenian
    {"sma", Lang::Sma},  // Southern Sami
    {"se", Lang::Sme},   // Northern Sami
    {"sm", Lang::Smo},   // Samoan
    {"sn", Lang::Sna},   // Shona
    {"sd", Lang::Snd},   // Sindhi
    {"so", Lang::Som},   // Somali
    {"st", Lang::Sot},   // Southern Sotho
    {"es", Lang::Spa},   // Spanish
    {"sq", Lang::Sqi},   // Albanian
    {"sc", Lang::Srd},   // Sardinian
    {"srn", Lang::Srn},  // Sranan Tongo
    {"sr", Lang::Srp},   // Serbian
    {"ss", Lang::Ssw},   // Swati
    {"stq", Lang::Stq},  // Saterfriesisch
    {"su", Lang::Sun},   // Sundanese
    {"sux", Lang::Sux},  // Sumerian
    {"sv", Lang::Swe},   // Swedish
    {"swg", Lang::Swg},  // Swabian
    {"sw", Lang::Swh},   // Swahili
    {"syc", Lang::Syc},  // Classical Syriac
    {"szl", Lang::Szl},  // Silesian
    {"ty", Lang::Tah},   // Tahitian
    {"ta", Lang::Tam},   // Tamil
    {"tt", Lang::Tat},   // Tatar
    {"te", Lang::Tel},   // Telugu
    {"tet", Lang::Tet},  // Tetum
    {"tg", Lang::Tgk},   // Tajik
    {"tl", Lang::Tgl},   // Tagalog
    {"th", Lang::Tha},   // Thai
    {"thv", Lang::Thv},  // Tahaggart Tamahaq
    {"tig", Lang::Tig},  // Tigre
    {"ti", Lang::Tir},   // Tigrinya
    {"tkl", Lang::Tkl},  // Tokelau
    {"tlh", Lang::Tlh},  // Klingon
    {"tly", Lang::Tly},  // Talysh
    {"tmr", Lang::Tmr},  // Jewish Babylonian Aramaic
    {"tmw", Lang::Tmw},  // Temuan
    {"toi", Lang::Toi},  // Tonga (Zambia)
    {"tok", Lang::Tok},  // Toki Pona
    {"to", Lang::Ton},   // Tonga (Tonga Islands)
    {"tpi", Lang::Tpi},  // Tok Pisin
    {"tpw", Lang::Tpw},  // Tupí
    {"tn", Lang::Tsn},   // Tswana
    {"ts", Lang::Tso},   // Tsonga
    {"tts", Lang::Tts},  // Northeastern Thai
    {"tk", Lang::Tuk},   // Turkmen
    {"tr", Lang::Tur},   // Turkish
    {"tvl", Lang::Tvl},  // Tuvalu
    {"tyv", Lang::Tyv},  // Tuvinian
    {"tzl", Lang::Tzl},  // Talossan
    {"udm", Lang::Udm},  // Udmurt
    {"ug", Lang::Uig},   // Uighur
    {"uk", Lang::Ukr},   // Ukrainian
    {"umb", Lang::Umb},  // Umbundu
    {"ur", Lang::Urd},   // Urdu
    {"urh", Lang::Urh},  // Urhobo
    {"uz", Lang::Uzb},   // Uzbek
    {"vec", Lang::Vec},  // Venetian
    {"vep", Lang::Vep},  // Veps
    {"vi", Lang::Vie},   // Vietnamese
    {"vo", Lang::Vol},   // Volapük
    {"vro", Lang::Vro},  // Võro
    {"war", Lang::War},  // Waray (Philippines)
    {"wa", Lang::Wln},   // Walloon
    {"wo", Lang::Wol},   // Wolof
    {"wuu", Lang::Wuu},  // Wu Chinese
    {"xal", Lang::Xal},  // Kalmyk
    {"xh", Lang::Xho},   // Xhosa
    {"xmf", Lang::Xmf},  // Mingrelian
    {"xqa", Lang::Xqa},  // Karakhanid
    {"yi", Lang::Yid},   // Yiddish
    {"yo", Lang::Yor},   // Yoruba
    {"yua", Lang::Yua},  // Yucateco
    {"yue", Lang::Yue},  // Yue Chinese
    {"zea", Lang::Zea},  // Zeeuws
    {"zgh", Lang::Zgh},  // Standard Moroccan Tamazight
    {"zlm", Lang::Zlm},  // Malay (individual language)
    {"ms", Lang::Zsm},   // Standard Malay
    {"zu", Lang::Zul},   // Zulu
    {"zza", Lang::Zza}   // Zaza
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
