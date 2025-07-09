#include <iostream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
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

// Language group definitions - languages in the same group are considered functionally equivalent
enum class LangGroup {
    ARABIC,       // Arabic variants
    CHINESE,      // Chinese dialects
    KURDISH,      // Kurdish variants
    NORWEGIAN,    // Norwegian variants
    MALAY,        // Malay variants
    MARI,         // Mari variants
    SAMI,         // Sami variants
    FRISIAN,      // Frisian variants
    SORBIAN,      // Sorbian variants
    FRENCH,       // French variants
    ENGLISH,      // English variants
    GERMAN,       // German variants
    NAHUATL,      // Nahuatl variants
    PANJABI,      // Panjabi variants
    FULFULDE,     // Fulfulde variants
    HAIDA,        // Haida variants
    HMONG,        // Hmong variants
    ARAMAIC,      // Aramaic variants
    MURUT,        // Murut variants
    KADAZAN,      // Kadazan variants
    TONGA,        // Tonga variants
    GREEK,        // Greek variants
    HEBREW,       // Hebrew variants
    CREE,         // Cree variants
    SPANISH,      // Spanish variants
    TURKISH,      // Turkish variants
    ALBANIAN,     // Albanian variants
    THAI,         // Thai variants
    BERBER,       // Berber variants
    FINNISH,      // Finnish variants
    KOMI,         // Komi variants
    LATVIAN,      // Latvian variants
    RUSSIAN,      // Russian variants
    OTHER         // Languages with no grouping (standalone)
};

// Map each language to its functional group
const std::unordered_map<Lang, LangGroup> langToGroupMap = {
    // Arabic variants
    {Lang::Ara, LangGroup::ARABIC}, {Lang::Acm, LangGroup::ARABIC}, {Lang::Afb, LangGroup::ARABIC},
    {Lang::Ajp, LangGroup::ARABIC}, {Lang::Apc, LangGroup::ARABIC}, {Lang::Arq, LangGroup::ARABIC},
    {Lang::Ary, LangGroup::ARABIC}, {Lang::Arz, LangGroup::ARABIC}, {Lang::Ayl, LangGroup::ARABIC},
    
    // Chinese variants
    {Lang::Cmn, LangGroup::CHINESE}, {Lang::Yue, LangGroup::CHINESE}, {Lang::Wuu, LangGroup::CHINESE},
    {Lang::Gan, LangGroup::CHINESE}, {Lang::Hak, LangGroup::CHINESE}, {Lang::Hsn, LangGroup::CHINESE},
    {Lang::Cjy, LangGroup::CHINESE}, {Lang::Lzh, LangGroup::CHINESE},
    
    // Kurdish variants
    {Lang::Ckb, LangGroup::KURDISH}, {Lang::Kmr, LangGroup::KURDISH}, {Lang::Sdh, LangGroup::KURDISH},
    {Lang::Kiu, LangGroup::KURDISH},
    
    // Norwegian variants
    {Lang::Nob, LangGroup::NORWEGIAN}, {Lang::Nno, LangGroup::NORWEGIAN},
    
    // Malay variants
    {Lang::Zsm, LangGroup::MALAY}, {Lang::Zlm, LangGroup::MALAY}, {Lang::Max, LangGroup::MALAY},
    
    // Mari variants
    {Lang::Mhr, LangGroup::MARI}, {Lang::Mrj, LangGroup::MARI},
    
    // Sami variants
    {Lang::Sma, LangGroup::SAMI}, {Lang::Sme, LangGroup::SAMI},
    
    // Frisian variants
    {Lang::Fry, LangGroup::FRISIAN}, {Lang::Frr, LangGroup::FRISIAN}, {Lang::Stq, LangGroup::FRISIAN},
    {Lang::Ofs, LangGroup::FRISIAN},
    
    // Sorbian variants
    {Lang::Dsb, LangGroup::SORBIAN}, {Lang::Hsb, LangGroup::SORBIAN},
    
    // French variants
    {Lang::Fra, LangGroup::FRENCH}, {Lang::Frm, LangGroup::FRENCH}, {Lang::Fro, LangGroup::FRENCH},
    
    // English variants
    {Lang::Eng, LangGroup::ENGLISH}, {Lang::Enm, LangGroup::ENGLISH}, {Lang::Ang, LangGroup::ENGLISH},
    
    // German variants
    {Lang::Deu, LangGroup::GERMAN}, {Lang::Bar, LangGroup::GERMAN}, {Lang::Gsw, LangGroup::GERMAN},
    {Lang::Pdc, LangGroup::GERMAN}, {Lang::Pfl, LangGroup::GERMAN}, {Lang::Swg, LangGroup::GERMAN},
    {Lang::Nds, LangGroup::GERMAN},
    
    // Nahuatl variants
    {Lang::Nah, LangGroup::NAHUATL}, {Lang::Nch, LangGroup::NAHUATL}, {Lang::Ngu, LangGroup::NAHUATL},
    {Lang::Nlv, LangGroup::NAHUATL},
    
    // Panjabi variants
    {Lang::Pan, LangGroup::PANJABI}, {Lang::Pnb, LangGroup::PANJABI},
    
    // Fulfulde variants
    {Lang::Fuc, LangGroup::FULFULDE}, {Lang::Fuv, LangGroup::FULFULDE},
    
    // Haida variants
    {Lang::Hax, LangGroup::HAIDA}, {Lang::Hdn, LangGroup::HAIDA},
    
    // Hmong variants
    {Lang::Hnj, LangGroup::HMONG}, {Lang::Mww, LangGroup::HMONG},
    
    // Aramaic variants
    {Lang::Oar, LangGroup::ARAMAIC}, {Lang::Jpa, LangGroup::ARAMAIC}, {Lang::Tmr, LangGroup::ARAMAIC},
    {Lang::Aii, LangGroup::ARAMAIC}, {Lang::Syc, LangGroup::ARAMAIC},
    
    // Murut variants
    {Lang::Kxi, LangGroup::MURUT}, {Lang::Mvv, LangGroup::MURUT},
    
    // Kadazan variants
    {Lang::Dtp, LangGroup::KADAZAN}, {Lang::Kzj, LangGroup::KADAZAN},
    
    // Tonga variants
    {Lang::Toi, LangGroup::TONGA}, {Lang::Ton, LangGroup::TONGA},
    
    // Greek variants
    {Lang::Ell, LangGroup::GREEK}, {Lang::Grc, LangGroup::GREEK},
    
    // Hebrew variants
    {Lang::Heb, LangGroup::HEBREW}, {Lang::Hbo, LangGroup::HEBREW},
    
    // Thai variants
    {Lang::Tha, LangGroup::THAI}, {Lang::Tts, LangGroup::THAI},
    
    // Berber variants
    {Lang::Ber, LangGroup::BERBER}, {Lang::Kab, LangGroup::BERBER}, {Lang::Shi, LangGroup::BERBER},
    {Lang::Shy, LangGroup::BERBER}, {Lang::Rif, LangGroup::BERBER}, {Lang::Zgh, LangGroup::BERBER},
    
    // Finnish variants
    {Lang::Fin, LangGroup::FINNISH}, {Lang::Fkv, LangGroup::FINNISH},
    
    // Komi variants
    {Lang::Koi, LangGroup::KOMI}, {Lang::Kpv, LangGroup::KOMI},
    
    // Spanish variants
    {Lang::Spa, LangGroup::SPANISH}, {Lang::Osp, LangGroup::SPANISH},
    
    // Turkish variants
    {Lang::Tur, LangGroup::TURKISH}, {Lang::Ota, LangGroup::TURKISH}, {Lang::Otk, LangGroup::TURKISH},
    
    // Albanian variants
    {Lang::Sqi, LangGroup::ALBANIAN}, {Lang::Aln, LangGroup::ALBANIAN},
    
    // Russian variants
    {Lang::Rus, LangGroup::RUSSIAN}, {Lang::Orv, LangGroup::RUSSIAN}
};

Lang codeToLang(const std::string& code) {
    auto it = codeToLangMap.find(code);
    if (it != codeToLangMap.end()) {
        return it->second;
    }
    return Lang::Eng; // default
}

LangGroup getLangGroup(Lang lang) {
    auto it = langToGroupMap.find(lang);
    if (it != langToGroupMap.end()) {
        return it->second;
    }
    return LangGroup::OTHER;
}

// Check if two languages are functionally equivalent (same group or exact match)
bool areFunctionallyEquivalent(Lang lang1, Lang lang2) {
    if (lang1 == lang2) {
        return true;
    }
    
    LangGroup group1 = getLangGroup(lang1);
    LangGroup group2 = getLangGroup(lang2);
    
    // Only consider them equivalent if they're in the same group AND it's not the OTHER group
    return (group1 == group2) && (group1 != LangGroup::OTHER);
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

// Get group name for display
std::string getGroupName(LangGroup group) {
    switch (group) {
        case LangGroup::ARABIC: return "Arabic";
        case LangGroup::CHINESE: return "Chinese";
        case LangGroup::KURDISH: return "Kurdish";
        case LangGroup::NORWEGIAN: return "Norwegian";
        case LangGroup::MALAY: return "Malay";
        case LangGroup::MARI: return "Mari";
        case LangGroup::SAMI: return "Sami";
        case LangGroup::FRISIAN: return "Frisian";
        case LangGroup::SORBIAN: return "Sorbian";
        case LangGroup::FRENCH: return "French";
        case LangGroup::ENGLISH: return "English";
        case LangGroup::GERMAN: return "German";
        case LangGroup::NAHUATL: return "Nahuatl";
        case LangGroup::PANJABI: return "Panjabi";
        case LangGroup::FULFULDE: return "Fulfulde";
        case LangGroup::HAIDA: return "Haida";
        case LangGroup::HMONG: return "Hmong";
        case LangGroup::ARAMAIC: return "Aramaic";
        case LangGroup::MURUT: return "Murut";
        case LangGroup::KADAZAN: return "Kadazan";
        case LangGroup::TONGA: return "Tonga";
        case LangGroup::GREEK: return "Greek";
        case LangGroup::HEBREW: return "Hebrew";
        case LangGroup::THAI: return "Thai";
        case LangGroup::BERBER: return "Berber";
        case LangGroup::FINNISH: return "Finnish";
        case LangGroup::KOMI: return "Komi";
        case LangGroup::SPANISH: return "Spanish";
        case LangGroup::TURKISH: return "Turkish";
        case LangGroup::ALBANIAN: return "Albanian";
        case LangGroup::RUSSIAN: return "Russian";
        case LangGroup::OTHER: return "Individual";
        default: return "Unknown";
    }
}

int main() {
    std::vector<std::string> labels = readLines("../data/labels.txt");
    std::vector<std::string> texts = readLines("../data/text.txt");
    
    if (labels.empty() || texts.empty() || labels.size() != texts.size()) {
        std::cerr << "Error loading files or size mismatch" << std::endl;
        return 1;
    }
    
    int total = 0, correct = 0, correctWithGrouping = 0, unknown = 0;
    std::unordered_map<std::string, int> errorsByLanguage;
    std::unordered_map<std::string, int> totalByLanguage;
    std::unordered_map<std::string, int> groupMatches; // Track group-level matches
    
    // Open files to write incorrect predictions
    std::ofstream errorFile("errors.txt");
    std::ofstream groupErrorFile("group_errors.txt");
    if (!errorFile.is_open() || !groupErrorFile.is_open()) {
        std::cerr << "Warning: Cannot create error files" << std::endl;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < labels.size(); ++i) {
        Lang expected = codeToLang(labels[i]);
        Lang detected = LanguageDetector::detectLanguage(texts[i]);
        
        // Count total samples per language
        totalByLanguage[labels[i]]++;
        
        if (expected == Lang::Eng && labels[i] != "en") {
            unknown++;
        } else {
            bool exactMatch = (detected == expected);
            bool functionalMatch = areFunctionallyEquivalent(detected, expected);
            
            if (exactMatch) {
                correct++;
                correctWithGrouping++;
            } else if (functionalMatch) {
                correctWithGrouping++;
                groupMatches[labels[i]]++;
                
                // Log group matches separately
                if (groupErrorFile.is_open()) {
                    LangGroup group = getLangGroup(expected);
                    groupErrorFile << "Group Match (" << getGroupName(group) << "): " 
                                  << labels[i] << " -> " << langToCode(detected) << std::endl;
                    groupErrorFile << "Text: " << texts[i] << std::endl;
                    groupErrorFile << "---" << std::endl;
                }
            } else {
                // Count errors per language (both exact and functional failures)
                errorsByLanguage[labels[i]]++;
                
                // Write incorrect prediction to file
                if (errorFile.is_open()) {
                    errorFile << "Expected: " << labels[i] << " (" << langToCode(expected) << ")" << std::endl;
                    errorFile << "Got: " << langToCode(detected) << std::endl;
                    errorFile << "Text: " << texts[i] << std::endl;
                    errorFile << "---" << std::endl;
                }
            }
        }
        
        total++;
    }
    
    errorFile.close();
    groupErrorFile.close();
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double exactAccuracy = (double)correct / total * 100.0;
    double functionalAccuracy = (double)correctWithGrouping / total * 100.0;
    double totalTime = duration.count() / 1000.0;
    double avgTime = totalTime / total;
    
    std::cout << "=== LANGUAGE DETECTION EVALUATION ===" << std::endl;
    std::cout << "Samples: " << total << std::endl;
    std::cout << "Exact matches: " << correct << std::endl;
    std::cout << "Functional matches: " << correctWithGrouping << std::endl;
    std::cout << "Group-level corrections: " << (correctWithGrouping - correct) << std::endl;
    std::cout << std::endl;
    std::cout << "Exact accuracy: " << std::fixed << std::setprecision(2) << exactAccuracy << "%" << std::endl;
    std::cout << "Functional accuracy: " << std::fixed << std::setprecision(2) << functionalAccuracy << "%" << std::endl;
    std::cout << "Improvement: +" << std::fixed << std::setprecision(2) << (functionalAccuracy - exactAccuracy) << "%" << std::endl;
    std::cout << std::endl;
    std::cout << "Total time: " << std::fixed << std::setprecision(2) << totalTime << " ms" << std::endl;
    std::cout << "Avg time: " << std::fixed << std::setprecision(4) << avgTime << " ms/sample" << std::endl;
    std::cout << std::endl;
    std::cout << "Errors written to: errors.txt" << std::endl;
    std::cout << "Group matches written to: group_errors.txt" << std::endl;
    
    // Calculate and display error statistics by language
    std::cout << "\n=== ERROR BREAKDOWN BY LANGUAGE ===" << std::endl;
    std::cout << std::setw(8) << "Lang" << std::setw(8) << "Errors" << std::setw(8) << "Total" 
              << std::setw(10) << "Error%" << std::setw(12) << "GroupFix" << std::endl;
    std::cout << "-----------------------------------------------------------" << std::endl;
    
    int totalErrors = total - correctWithGrouping - unknown;
    
    // Create vector of pairs for sorting
    std::vector<std::pair<std::string, int>> errorPairs(errorsByLanguage.begin(), errorsByLanguage.end());
    
    // Sort by number of errors (descending)
    std::sort(errorPairs.begin(), errorPairs.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& pair : errorPairs) {
        const std::string& lang = pair.first;
        int errors = pair.second;
        int totalSamples = totalByLanguage[lang];
        int groupFixed = groupMatches[lang];
        double errorPercent = (double)errors / totalSamples * 100.0;
        
        std::cout << std::setw(8) << lang 
                  << std::setw(8) << errors 
                  << std::setw(8) << totalSamples 
                  << std::setw(9) << std::fixed << std::setprecision(1) << errorPercent << "%" 
                  << std::setw(12) << groupFixed << std::endl;
    }
    
    // Show languages that benefited from grouping
    std::cout << "\n=== LANGUAGES IMPROVED BY GROUPING ===" << std::endl;
    std::vector<std::pair<std::string, int>> groupPairs(groupMatches.begin(), groupMatches.end());
    std::sort(groupPairs.begin(), groupPairs.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    for (const auto& pair : groupPairs) {
        if (pair.second > 0) {
            const std::string& lang = pair.first;
            Lang langEnum = codeToLang(lang);
            LangGroup group = getLangGroup(langEnum);
            std::cout << lang << ": +" << pair.second << " (" << getGroupName(group) << " group)" << std::endl;
        }
    }
    
    return 0;
}
