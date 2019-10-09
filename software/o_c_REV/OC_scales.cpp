#include "OC_scales.h"
#include "OC_options.h"
#include "braids_quantizer_scales.h"


namespace OC {

Scale user_scales[Scales::SCALE_USER_LAST];
Scale dummy_scale;

/*static*/
const int Scales::NUM_SCALES = OC::Scales::SCALE_USER_LAST + sizeof(braids::scales) / sizeof(braids::scales[0]);

/*static*/
void Scales::Init() {
  for (size_t i = 0; i < SCALE_USER_LAST; ++i)
    memcpy(&user_scales[i], &braids::scales[1], sizeof(Scale));
}

/*static*/
const Scale &Scales::GetScale(int index) {
  if (index < SCALE_USER_LAST)
    return user_scales[index];
  else
    return braids::scales[index - SCALE_USER_LAST];
}

const char* const scale_names_short[] = {
    "USER1",
    "USER2",
    "USER3",
    "USER4",
    "OFF ",
    "SEMI",
    "IONI",
    "DORI",
    "PHRY",
    "LYDI",
    "MIXO",
    "AEOL",
    "LOCR",
    "BLU+",
    "BLU-",
    "PEN+",
    "PEN-",
    "FOLK",
    "JAPA",
    "GAME",
    "GYPS",
    "ARAB",
    "FLAM",
    "WHOL",
    "PYTH",
    "EB/4",
    "E /4",
    "EA/4",
    "BHAI",
    "GUNA",
    "MARW",
    "SHRI",
    "PURV",
    "BILA",
    "YAMA",
    "KAFI",
    "BHIM",
    // "DARB",
    // "RAGE",
    // "KHAM",
    // "MIMA",
    // "PARA",
    // "RANG",
    // "GANG",
    // "KAME",
    // "PAKA",
    // "NATB",
    // "KAUN",
    "BAIR",
    // "BTOD",
    // "CHAN",
    // "KTOD",
    // "JOGE",
    "VALL",
    "1322",
    "MANF",
    "MAGC",
    "QUAR",
    "ARMO",
    "HIRA",
    "SCOT",
    "THAI",
    
    "SEVI",
    "MACH",
    "FATH",
    "BLAC",
    "MAV7",
    "MAV9",
    "SPYT",
    
    #ifndef VOR
    "22OR",
    "PAJS",
    "PAJP",
    "PORC",
    "FLAT",
    "LEMB",
    "SENS",
    "53OR",
    "1272",
    "TRIZ",
    "2028",
    "MADG",
    "MARV",
    "PARA",
    "16ED",
    "15ED",
    "14ED",
    "13ED",
    "11ED",
    "10ED",
    "9ED",
    "8ED",
    "7ED",
    "6ED",
    "5ED",
    "16H2",
    "15H2",
    "14H2",
    "13H2",
    "12H2",
    "11H2",
    "10H2",
    "9H2",
    "8H2",
    "7H2",
    "6H2",
    "5H2",
    "4H2",
    #endif
//    "16S2",
//    "15S2",
//    "14S2",
//    "13S2",
//    "12S2",
//    "11S2",
//    "10S2",
//    "9S2",
//    "8S2",
//    "7S2",
//    "6S2",
//    "5S2",
//    "4S2",

    "B-Pe",
    "B-Pj",
    "B-Pl",
//    "16H3",
//    "14H3",
//    "12H3", 
//    "10H3", 
//    "8H3", 
//    "16S3",
//    "14S3",
//    "12S3", 
//    "10S3", 
//    "8S3"  
    };

const char* const scale_names[] = {
    "User-defined 1",
    "User-defined 2",
    "User-defined 3",
    "User-defined 4",
    "Off ",
    "Semitone",
    "Ionian",
    "Dorian",
    "Phrygian",
    "Lydian",
    "Mixolydian",
    "Aeolian",
    "Locrian",
    "Blues major",
    "Blues minor",
    "Pentatonic maj",
    "Pentatonic min",
    "Folk",
    "Japanese",
    "Gamelan",
    "Gypsy",
    "Arabian",
    "Flamenco",
    "Whole tone",
    "Pythagorean",
    "EB/4",
    "E /4",
    "EA/4",
    "Bhairav",
    "Gunakri",
    "Marwa",
    "Shree [Camel]",
    "Purvi",
    "Bilawal",
    "Yaman",
    "Kafi",
    "Bhimpalasree",
    // "Darbari",
    // "Rageshree",
    // "Khamaj",
    // "Mimal",
    // "Parameshwari",
    // "Rangeshwari",
    // "Gangeshwari",
    // "Kameshwari",
    // "Pa Khafi",
    // "Natbhairav",
    // "Malkauns",
    "Bairagi",
    // "B Todi",
    // "Chandradeep",
    // "Kaushik Todi",
    // "Jogeshwari",

    "Tart.-Vallotti",
    "13of22tETgen=5",
    "Mandelbaum",
    "Magic-in-145tET",
    "Quartaminor3rds",
    "Armodue semi-eq",
    "Hirajoshi",
    "Scot bagpipes",
    "Thai ranat",
    
    "Sevish12on31EDO",
    "11tetMachine6",
    "13tetFather8",
    "15tetBlackwd10",
    "16tetMavila7",
    "16tetMavila9",
    "17tetSuprpyth12",
    
    #ifndef VOR
    "22tetOrwell9",
    "22tetPajaraSy10",
    "22tetPajara5-10",
    "22tetPorcupine7",
    "26tetFlattone12",
    "26tetLemba10",
    "46tetSensi11",
    "53tetOrwell9",
    "12of72tetRodgrs",
    "TrivalentZeus7",
    "202tetOctone",
    "313tetElfMadag9",
    "MarvelWooGlumma",
    "TOP Parapyth12",
    "16-ED (2 or 3)",
    "15-ED (2 or 3)",
    "14-ED (2 or 3)",
    "13-ED (2 or 3)",
    "11-ED (2 or 3)",
    "10-ED (2 or 3)",
    "9-ED (2 or 3)",
    "8-ED (2 or 3)",
    "7-ED (2 or 3)",
    "6-ED2",
    "5-ED2",
    "16-HD2",
    "15-HD2",
    "14-HD2",
    "13-HD2",
    "12-HD2",
    "11-HD2",
    "10-HD2",
    "9-HD2",
    "8-HD2",
    "7-HD2",
    "6-HD2",
    "5-HD2",
    "4-HD2",
    #endif
    
//    "16-SD2",
//    "15-SD2",
//    "14-SD2",
//    "13-SD2",
//    "12-SD2",
//    "11-SD2",
//    "10-SD2",
//    "9-SD2",
//    "8-SD2",
//    "7-SD2",
//    "6-SD2",
//    "5-SD2",
//    "4-SD2",
    
    "Bohlen-Pierce =",
    "Bohlen-Pierce j",
    "Bohlen-Pierce l",
//    "8-24-HD3[16]",
//    "7-21-HD3[14]",
//    "6-18-HD3[12]", 
//    "5-15-HD3[10]", 
//    "4-12-HD3[8]", 
//    "24-8-SD3[16]",
//    "21-7-SD3[14]",
//    "18-6-SD3[12]", 
//    "15-5-SD3[10]", 
//    "12-4-SD3[8]"
    };

const char* const voltage_scalings[] = {
    " 1V/O", // 1V/octave
    "alpha", // Wendy Carlos alpha scale
    " beta", // Wendy Carlos beta scale
    "gamma", // Wendy Carlos gamma scale
    "  tri", // Tritave (as used by Bohlen-Pierce macrotonal scale)
    "  qtr", // quartertone scale (essentially dowbscales to 0.5V/oct)
    "1.2/O", // 1.2V/octave Buchla
    " 2V/O"  // 2V/oct Buchla
    } ;

}; // namespace OC
