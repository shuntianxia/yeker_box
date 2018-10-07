
#include "bitstream.h"

#include "window1251.h"
typedef struct __language_coding
{
	char language[4];
	int  coding;
}anguage_coding_s;
static const anguage_coding_s ISO_639_2_Code[545]=
 {//ISO_639_2_Code						//English name of Language                                                        
{"aar",-1},			              //Afar                                                                            
{"abk",-1},                    //Abkhazian                                                                       
{"ace",-1},                    //Achinese                                                                        
{"ach",-1},                    //Acoli                                                                           
{"ada",-1},                    //Adangme                                                                         
{"ady",-1},                    //Adyghe; Adygei                                                                  
{"afa",-1},                    //Afro-Asiatic languages                                                          
{"afh",-1},                    //Afrihili                                                                        
{"afr",-1},                    //Afrikaans                                                                       
{"ain",-1},                    //Ainu                                                                            
{"aka",-1},                    //Akan                                                                            
{"akk",-1},                    //Akkadian                                                                        
{"alb",-1},                    //Albanian                                                                        
{"sqi",-1},                    //                                                            
{"ale",-1},                    //Aleut                                                                           
{"alg",-1},                    //Algonquian languages                                                            
{"alt",-1},                    //Southern Altai                                                                  
{"amh",-1},                    //Amharic                                                                         
{"ang",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //English, Old (ca.450-1100)                                                     
{"anp",-1},                    //Angika                                                                          
{"apa",-1},                    //Apache languages                                                                
{"ara",-1},                    //Arabic                                                                          
{"arc",-1},                    //Official Aramaic (700-300 BCE); Imperial Aramaic (700-300 BCE)                  
{"arg",-1},                    //Aragonese                                                                       
{"arm",-1},                    //Armenian                                                                        
{"hye",-1},                    //                                                            
{"arn",-1},                    //Mapudungun; Mapuche                                                             
{"arp",-1},                    //Arapaho                                                                         
{"art",-1},                    //Artificial languages                                                            
{"arw",-1},                    //Arawak                                                                          
{"asm",-1},                    //Assamese                                                                        
{"ast",-1},                    //Asturian; Bable; Leonese; Asturleonese                                          
{"ath",-1},                    //Athapascan languages                                                            
{"aus",-1},                    //Australian languages                                                            
{"ava",-1},                    //Avaric                                                                          
{"ave",-1},                    //Avestan                                                                         
{"awa",-1},                    //Awadhi                                                                          
{"aym",-1},                    //Aymara                                                                          
{"aze",-1},                    //Azerbaijani                                                                     
{"bad",-1},                    //Banda languages                                                                 
{"bai",-1},                    //Bamileke languages                                                              
{"bak",-1},                    //Bashkir                                                                         
{"bal",-1},                    //Baluchi                                                                         
{"bam",-1},                    //Bambara                                                                         
{"ban",-1},                    //Balinese                                                                        
{"baq",-1},                    //Basque                                                                          
{"eus",-1},                    //                                                            
{"bas",-1},                    //Basa                                                                            
{"bat",-1},                    //Baltic languages                                                                
{"bej",-1},                    //Beja; Bedawiyet                                                                 
{"bel",A_AUDIO_FONTTYPE_ISOIEC8859_5},                    //Belarusian                                                                      
{"bem",-1},                    //Bemba                                                                           
{"ben",-1},                    //Bengali                                                                         
{"ber",-1},                    //Berber languages                                                                
{"bho",-1},                    //Bhojpuri                                                                        
{"bih",-1},                    //Bihari languages                                                                
{"bik",-1},                    //Bikol                                                                           
{"bin",-1},                    //Bini; Edo                                                                       
{"bis",-1},                    //Bislama                                                                         
{"bla",-1},                    //Siksika                                                                         
{"bnt",-1},                    //Bantu languages                                                                 
{"tib",-1},                    //Tibetan                                                                         
{"bod",-1},                    //                                                            
{"bos",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Bosnian                                                                         
{"bra",-1},                    //Braj                                                                            
{"bre",-1},                    //Breton                                                                          
{"btk",-1},                    //Batak languages                                                                 
{"bua",-1},                    //Buriat                                                                          
{"bug",-1},                    //Buginese                                                                        
{"bul",A_AUDIO_FONTTYPE_ISOIEC8859_5},                    //Bulgarian                                                                       
{"bur",-1},                    //Burmese                                                                         
{"mya",-1},                    //                                                            
{"byn",-1},                    //Blin; Bilin                                                                     
{"cad",-1},                    //Caddo                                                                           
{"cai",-1},                    //Central American Indian languages                                               
{"car",-1},                    //Galibi Carib                                                                    
{"cat",-1},                    //Catalan; Valencian                                                              
{"cau",-1},                    //Caucasian languages                                                             
{"ceb",-1},                    //Cebuano                                                                         
{"cel",-1},                    //Celtic languages                                                                
{"cze",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Czech                                                                           
{"ces",-1},                    //                                                            
{"cha",-1},                    //Chamorro                                                                        
{"chb",-1},                    //Chibcha                                                                         
{"che",-1},                    //Chechen                                                                         
{"chg",-1},                    //Chagatai                                                                        
{"chi",-1},                    //Chinese                                                                         
{"zho",-1},                    //                                                            
{"chk",-1},                    //Chuukese                                                                        
{"chm",-1},                    //Mari                                                                            
{"chn",-1},                    //Chinook jargon                                                                  
{"cho",-1},                    //Choctaw                                                                         
{"chp",-1},                    //Chipewyan; Dene Suline                                                          
{"chr",-1},                    //Cherokee                                                                        
{"chu",-1},                    //Church Slavic; Old Slavonic; Church Slavonic; Old Bulgarian; Old Church Slavonic
{"chv",-1},                    //Chuvash                                                                         
{"chy",-1},                    //Cheyenne                                                                        
{"cmc",-1},                    //Chamic languages                                                                
{"cop",-1},                    //Coptic                                                                          
{"cor",-1},                    //Cornish                                                                         
{"cos",-1},                    //Corsican                                                                        
{"cpe",-1},                    //Creoles and pidgins, English based                                              
{"cpf",-1},                    //Creoles and pidgins, French-based                                               
{"cpp",-1},                    //Creoles and pidgins, Portuguese-based                                           
{"cre",-1},                    //Cree                                                                            
{"crh",-1},                    //Crimean Tatar; Crimean Turkish                                                  
{"crp",-1},                    //Creoles and pidgins                                                             
{"csb",-1},                    //Kashubian                                                                       
{"cus",-1},                    //Cushitic languages                                                              
{"wel",-1},                    //Welsh                                                                           
{"cym",-1},                    //                                                            
{"cze",-1},                    //Czech                                                                           
{"ces",-1},                    //                                                            
{"dak",-1},                    //Dakota                                                                          
{"dan",-1},                    //Danish                                                                          
{"dar",-1},                    //Dargwa                                                                          
{"day",-1},                    //Land Dayak languages                                                            
{"del",-1},                    //Delaware                                                                        
{"den",-1},                    //Slave (Athapascan)                                                              
{"ger",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //German                                                                          
{"deu",-1},                    //                                                            
{"dgr",-1},                    //Dogrib                                                                          
{"din",-1},                    //Dinka                                                                           
{"div",-1},                    //Divehi; Dhivehi; Maldivian                                                      
{"doi",-1},                    //Dogri                                                                           
{"dra",-1},                    //Dravidian languages                                                             
{"dsb",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Lower Sorbian                                                                   
{"dua",-1},                    //Duala                                                                           
{"dum",-1},                    //Dutch, Middle (ca.1050-1350)                                                    
{"dut",-1},                    //Dutch; Flemish                                                                  
{"nld",-1},                    //                                                            
{"dyu",-1},                    //Dyula                                                                           
{"dzo",-1},                    //Dzongkha                                                                        
{"efi",-1},                    //Efik                                                                            
{"egy",-1},                    //Egyptian (Ancient)                                                              
{"eka",-1},                    //Ekajuk                                                                          
{"gre",-1},                    //Greek, Modern (1453-)                                                           
{"ell",-1},                    //                                                            
{"elx",-1},                    //Elamite                                                                         
{"eng",-1},                    //English                                                                         
{"enm",-1},                    //English, Middle (1100-1500)                                                     
{"epo",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Esperanto                                                                       
{"est",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Estonian                                                                        
{"baq",-1},                    //Basque                                                                          
{"eus",-1},                    //                                                            
{"ewe",-1},                    //Ewe                                                                             
{"ewo",-1},                    //Ewondo                                                                          
{"fan",-1},                    //Fang                                                                            
{"fao",-1},                    //Faroese                                                                         
{"per",-1},                    //Persian                                                                         
{"fas",-1},                    //                                                            
{"fat",-1},                    //Fanti                                                                           
{"fij",-1},                    //Fijian                                                                          
{"fil",-1},                    //Filipino; Pilipino                                                              
{"fin",-1},                    //Finnish                                                                         
{"fiu",-1},                    //Finno-Ugrian languages                                                          
{"fon",-1},                    //Fon                                                                             
{"fre",-1},                    //French                                                                          
{"fra",-1},                    //                                                            
{"fre",-1},                    //French                                                                          
{"fra",-1},                    //                                                            
{"frm",-1},                    //French, Middle (ca.1400-1600)                                                   
{"fro",-1},                    //French, Old (842-ca.1400)                                                       
{"frr",-1},                    //Northern Frisian                                                                
{"frs",-1},                    //Eastern Frisian                                                                 
{"fry",-1},                    //Western Frisian                                                                 
{"ful",-1},                    //Fulah                                                                           
{"fur",-1},                    //Friulian                                                                        
{"gaa",-1},                    //Ga                                                                              
{"gay",-1},                    //Gayo                                                                            
{"gba",-1},                    //Gbaya                                                                           
{"gem",-1},                    //Germanic languages                                                              
{"geo",-1},                    //Georgian                                                                        
{"kat",-1},                    //                                                            
{"ger",-1},                    //German                                                                          
{"deu",-1},                    //                                                            
{"gez",-1},                    //Geez                                                                            
{"gil",-1},                    //Gilbertese                                                                      
{"gla",-1},                    //Gaelic; Scottish Gaelic                                                         
{"gle",-1},                    //Irish                                                                           
{"glg",-1},                    //Galician                                                                        
{"glv",-1},                    //Manx                                                                            
{"gmh",-1},                    //German, Middle High (ca.1050-1500)                                              
{"goh",-1},                    //German, Old High (ca.750-1050)                                                  
{"gon",-1},                    //Gondi                                                                           
{"gor",-1},                    //Gorontalo                                                                       
{"got",-1},                    //Gothic                                                                          
{"grb",-1},                    //Grebo                                                                           
{"grc",-1},                    //Greek, Ancient (to 1453)                                                        
{"gre",-1},                    //Greek, Modern (1453-)                                                           
{"ell",-1},                    //                                                            
{"grn",-1},                    //Guarani                                                                         
{"gsw",-1},                    //Swiss German; Alemannic; Alsatian                                               
{"guj",-1},                    //Gujarati                                                                        
{"gwi",-1},                    //Gwich'in                                                                        
{"hai",-1},                    //Haida                                                                           
{"hat",-1},                    //Haitian; Haitian Creole                                                         
{"hau",-1},                    //Hausa                                                                           
{"haw",-1},                    //Hawaiian                                                                        
{"heb",-1},                    //Hebrew                                                                          
{"her",-1},                    //Herero                                                                          
{"hil",-1},                    //Hiligaynon                                                                      
{"him",-1},                    //Himachali languages; Western Pahari languages                                   
{"hin",-1},                    //Hindi                                                                           
{"hit",-1},                    //Hittite                                                                         
{"hmn",-1},                    //Hmong; Mong                                                                     
{"hmo",-1},                    //Hiri Motu                                                                       
{"hrv",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Croatian                                                                        
{"hsb",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Upper Sorbian                                                                   
{"hun",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Hungarian                                                                       
{"hup",-1},                    //Hupa                                                                            
{"arm",-1},                    //Armenian                                                                        
{"hye",-1},                    //                                                            
{"iba",-1},                    //Iban                                                                            
{"ibo",-1},                    //Igbo                                                                            
{"ice",-1},                    //Icelandic                                                                       
{"isl",-1},                    //                                                            
{"ido",-1},                    //Ido                                                                             
{"iii",-1},                    //Sichuan Yi; Nuosu                                                               
{"ijo",-1},                    //Ijo languages                                                                   
{"iku",-1},                    //Inuktitut                                                                       
{"ile",-1},                    //Interlingue; Occidental                                                         
{"ilo",-1},                    //Iloko                                                                           
{"ina",-1},                    //Interlingua (International Auxiliary Language Association)                      
{"inc",-1},                    //Indic languages                                                                 
{"ind",-1},                    //Indonesian                                                                      
{"ine",-1},                    //Indo-European languages                                                         
{"inh",-1},                    //Ingush                                                                          
{"ipk",-1},                    //Inupiaq                                                                         
{"ira",-1},                    //Iranian languages                                                               
{"iro",-1},                    //Iroquoian languages                                                             
{"ice",-1},                    //Icelandic                                                                       
{"isl",-1},                    //                                                            
{"ita",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Italian                                                                         
{"jav",-1},                    //Javanese                                                                        
{"jbo",-1},                    //Lojban                                                                          
{"jpn",-1},                    //Japanese                                                                        
{"jpr",-1},                    //Judeo-Persian                                                                   
{"jrb",-1},                    //Judeo-Arabic                                                                    
{"kaa",-1},                    //Kara-Kalpak                                                                     
{"kab",-1},                    //Kabyle                                                                          
{"kac",-1},                    //Kachin; Jingpho                                                                 
{"kal",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Kalaallisut; Greenlandic                                                        
{"kam",-1},                    //Kamba                                                                           
{"kan",-1},                    //Kannada                                                                         
{"kar",-1},                    //Karen languages                                                                 
{"kas",-1},                    //Kashmiri                                                                        
{"geo",-1},                    //Georgian                                                                        
{"kat",-1},                    //                                                            
{"kau",-1},                    //Kanuri                                                                          
{"kaw",-1},                    //Kawi                                                                            
{"kaz",-1},                    //Kazakh                                                                          
{"kbd",-1},                    //Kabardian                                                                       
{"kha",-1},                    //Khasi                                                                           
{"khi",-1},                    //Khoisan languages                                                               
{"khm",-1},                    //Central Khmer                                                                   
{"kho",-1},                    //Khotanese; Sakan                                                                
{"kik",-1},                    //Kikuyu; Gikuyu                                                                  
{"kin",-1},                    //Kinyarwanda                                                                     
{"kir",-1},                    //Kirghiz; Kyrgyz                                                                 
{"kmb",-1},                    //Kimbundu                                                                        
{"kok",-1},                    //Konkani                                                                         
{"kom",-1},                    //Komi                                                                            
{"kon",-1},                    //Kongo                                                                           
{"kor",-1},                    //Korean                                                                          
{"kos",-1},                    //Kosraean                                                                        
{"kpe",-1},                    //Kpelle                                                                          
{"krc",-1},                    //Karachay-Balkar                                                                 
{"krl",-1},                    //Karelian                                                                        
{"kro",-1},                    //Kru languages                                                                   
{"kru",-1},                    //Kurukh                                                                          
{"kua",-1},                    //Kuanyama; Kwanyama                                                              
{"kum",-1},                    //Kumyk                                                                           
{"kur",-1},                    //Kurdish                                                                         
{"kut",-1},                    //Kutenai                                                                         
{"lad",-1},                    //Ladino                                                                          
{"lah",-1},                    //Lahnda                                                                          
{"lam",-1},                    //Lamba                                                                           
{"lao",-1},                    //Lao                                                                             
{"lat",-1},                    //Latin                                                                           
{"lav",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Latvian                                                                         
{"lez",-1},                    //Lezghian                                                                        
{"lim",-1},                    //Limburgan; Limburger; Limburgish                                                
{"lin",-1},                    //Lingala                                                                         
{"lit",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Lithuanian                                                                      
{"lol",-1},                    //Mongo                                                                           
{"loz",-1},                    //Lozi                                                                            
{"ltz",-1},                    //Luxembourgish; Letzeburgesch                                                    
{"lua",-1},                    //Luba-Lulua                                                                      
{"lub",-1},                    //Luba-Katanga                                                                    
{"lug",-1},                    //Ganda                                                                           
{"lui",-1},                    //Luiseno                                                                         
{"lun",-1},                    //Lunda                                                                           
{"luo",-1},                    //Luo (Kenya and Tanzania)                                                        
{"lus",-1},                    //Lushai                                                                          
{"mac",-1},                    //Macedonian                                                                      
{"mkd",-1},                    //                                                            
{"mad",-1},                    //Madurese                                                                        
{"mag",-1},                    //Magahi                                                                          
{"mah",-1},                    //Marshallese                                                                     
{"mai",-1},                    //Maithili                                                                        
{"mak",-1},                    //Makasar                                                                         
{"mal",-1},                    //Malayalam                                                                       
{"man",-1},                    //Mandingo                                                                        
{"mao",-1},                    //Maori                                                                           
{"mri",-1},                    //                                                            
{"map",-1},                    //Austronesian languages                                                          
{"mar",-1},                    //Marathi                                                                         
{"mas",-1},                    //Masai                                                                           
{"may",-1},                    //Malay                                                                           
{"msa",-1},                    //                                                            
{"mdf",-1},                    //Moksha                                                                          
{"mdr",-1},                    //Mandar                                                                          
{"men",-1},                    //Mende                                                                           
{"mga",-1},                    //Irish, Middle (900-1200)                                                        
{"mic",-1},                    //Mi'kmaq; Micmac                                                                 
{"min",-1},                    //Minangkabau                                                                     
{"mis",-1},                    //Uncoded languages                                                               
{"mac",-1},                    //Macedonian                                                                      
{"mkd",-1},                    //                                                            
{"mkh",-1},                    //Mon-Khmer languages                                                             
{"mlg",-1},                    //Malagasy                                                                        
{"mlt",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Maltese                                                                         
{"mnc",-1},                    //Manchu                                                                          
{"mni",-1},                    //Manipuri                                                                        
{"mno",-1},                    //Manobo languages                                                                
{"moh",-1},                    //Mohawk                                                                          
{"mon",-1},                    //Mongolian                                                                       
{"mos",-1},                    //Mossi                                                                           
{"mao",-1},                    //Maori                                                                           
{"mri",-1},                    //                                                            
{"may",-1},                    //Malay                                                                           
{"msa",-1},                    //                                                            
{"mul",-1},                    //Multiple languages                                                              
{"mun",-1},                    //Munda languages                                                                 
{"mus",-1},                    //Creek                                                                           
{"mwl",-1},                    //Mirandese                                                                       
{"mwr",-1},                    //Marwari                                                                         
{"bur",-1},                    //Burmese                                                                         
{"mya",-1},                    //                                                            
{"myn",-1},                    //Mayan languages                                                                 
{"myv",-1},                    //Erzya                                                                           
{"nah",-1},                    //Nahuatl languages                                                               
{"nai",-1},                    //North American Indian languages                                                 
{"nap",-1},                    //Neapolitan                                                                      
{"nau",-1},                    //Nauru                                                                           
{"nav",-1},                    //Navajo; Navaho                                                                  
{"nbl",-1},                    //Ndebele, South; South Ndebele                                                   
{"nde",-1},                    //Ndebele, North; North Ndebele                                                   
{"ndo",-1},                    //Ndonga                                                                          
{"nds",-1},                    //Low German; Low Saxon; German, Low; Saxon, Low                                  
{"nep",-1},                    //Nepali                                                                          
{"new",-1},                    //Nepal Bhasa; Newari                                                             
{"nia",-1},                    //Nias                                                                            
{"nic",-1},                    //Niger-Kordofanian languages                                                     
{"niu",-1},                    //Niuean                                                                          
{"dut",-1},                    //Dutch; Flemish                                                                  
{"nld",-1},                    //                                                            
{"nno",-1},                    //Norwegian Nynorsk; Nynorsk, Norwegian                                           
{"nob",-1},                    //Bokm?l, Norwegian; Norwegian Bokm?l                                             
{"nog",-1},                    //Nogai                                                                           
{"non",-1},                    //Norse, Old                                                                      
{"nor",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Norwegian                                                                       
{"nqo",-1},                    //N'Ko                                                                            
{"nso",-1},                    //Pedi; Sepedi; Northern Sotho                                                    
{"nub",-1},                    //Nubian languages                                                                
{"nwc",-1},                    //Classical Newari; Old Newari; Classical Nepal Bhasa                             
{"nya",-1},                    //Chichewa; Chewa; Nyanja                                                         
{"nym",-1},                    //Nyamwezi                                                                        
{"nyn",-1},                    //Nyankole                                                                        
{"nyo",-1},                    //Nyoro                                                                           
{"nzi",-1},                    //Nzima                                                                           
{"oci",-1},                    //Occitan (post 1500)                                                             
{"oji",-1},                    //Ojibwa                                                                          
{"ori",-1},                    //Oriya                                                                           
{"orm",-1},                    //Oromo                                                                           
{"osa",-1},                    //Osage                                                                           
{"oss",-1},                    //Ossetian; Ossetic                                                               
{"ota",-1},                    //Turkish, Ottoman (1500-1928)                                                    
{"oto",-1},                    //Otomian languages                                                               
{"paa",-1},                    //Papuan languages                                                                
{"pag",-1},                    //Pangasinan                                                                      
{"pal",-1},                    //Pahlavi                                                                         
{"pam",-1},                    //Pampanga; Kapampangan                                                           
{"pan",-1},                    //Panjabi; Punjabi                                                                
{"pap",-1},                    //Papiamento                                                                      
{"pau",-1},                    //Palauan                                                                         
{"peo",-1},                    //Persian, Old (ca.600-400 B.C.)                                                  
{"per",-1},                    //Persian                                                                         
{"fas",-1},                    //                                                            
{"phi",-1},                    //Philippine languages                                                            
{"phn",-1},                    //Phoenician                                                                      
{"pli",-1},                    //Pali                                                                            
{"pol",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Polish                                                                          
{"pon",-1},                    //Pohnpeian                                                                       
{"por",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Portuguese                                                                      
{"pra",-1},                    //Prakrit languages                                                               
{"pro",-1},                    //Proven?al, Old (to 1500);Occitan, Old (to 1500)                                 
{"pus",-1},                    //Pushto; Pashto                                                                  
{"qaa",-1},                    //Reserved for local use                                                          
{"que",-1},                    //Quechua                                                                         
{"raj",-1},                    //Rajasthani                                                                      
{"rap",-1},                    //Rapanui                                                                         
{"rar",-1},                    //Rarotongan; Cook Islands Maori                                                  
{"roa",-1},                    //Romance languages                                                               
{"roh",-1},                    //Romansh                                                                         
{"rom",-1},                    //Romany                                                                          
{"rum",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Romanian; Moldavian; Moldovan                                                   
{"ron",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"rum",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Romanian; Moldavian; Moldovan                                                   
{"ron",-1},                    //                                                            
{"run",-1},                    //Rundi                                                                           
{"rup",-1},                    //Aromanian; Arumanian; Macedo-Romanian                                           
{"rus",A_AUDIO_FONTTYPE_WINDOWS_1251},                    //Russian                                                                         
{"sad",-1},                    //Sandawe                                                                         
{"sag",-1},                    //Sango                                                                           
{"sah",-1},                    //Yakut                                                                           
{"sai",-1},                    //South American Indian languages                                                 
{"sal",-1},                    //Salishan languages                                                              
{"sam",-1},                    //Samaritan Aramaic                                                               
{"san",-1},                    //Sanskrit                                                                        
{"sas",-1},                    //Sasak                                                                           
{"sat",-1},                    //Santali                                                                         
{"scn",-1},                    //Sicilian                                                                        
{"sco",-1},                    //Scots                                                                           
{"sel",-1},                    //Selkup                                                                          
{"sem",-1},                    //Semitic languages                                                               
{"sga",-1},                    //Irish, Old (to 900)                                                             
{"sgn",-1},                    //Sign Languages                                                                  
{"shn",-1},                    //Shan                                                                            
{"sid",-1},                    //Sidamo                                                                          
{"sin",-1},                    //Sinhala; Sinhalese                                                              
{"sio",-1},                    //Siouan languages                                                                
{"sit",-1},                    //Sino-Tibetan languages                                                          
{"sla",-1},                    //Slavic languages                                                                
{"slo",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovak                                                                          
{"slk",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"slo",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovak                                                                          
{"slk",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //                                                            
{"slv",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Slovenian //maybe slovene????                                                                      
{"sma",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Southern Sami                                                                   
{"sme",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Northern Sami                                                                   
{"smi",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Sami languages                                                                  
{"smj",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Lule Sami                                                                       
{"smn",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Inari Sami                                                                      
{"smo",-1},                    //Samoan                                                                          
{"sms",A_AUDIO_FONTTYPE_ISOIEC8859_4},                    //Skolt Sami                                                                      
{"sna",-1},                    //Shona                                                                           
{"snd",-1},                    //Sindhi                                                                          
{"snk",-1},                    //Soninke                                                                         
{"sog",-1},                    //Sogdian                                                                         
{"som",-1},                    //Somali                                                                          
{"son",-1},                    //Songhai languages                                                               
{"sot",-1},                    //Sotho, Southern                                                                 
{"spa",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Spanish; Castilian                                                              
{"alb",-1},                    //Albanian                                                                        
{"sqi",-1},                    //                                                            
{"srd",-1},                    //Sardinian                                                                       
{"srn",-1},                    //Sranan Tongo                                                                    
{"srp",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Serbian                                                                         
{"srr",-1},                    //Serer                                                                           
{"ssa",-1},                    //Nilo-Saharan languages                                                          
{"ssw",-1},                    //Swati                                                                           
{"suk",-1},                    //Sukuma                                                                          
{"sun",-1},                    //Sundanese                                                                       
{"sus",-1},                    //Susu                                                                            
{"sux",-1},                    //Sumerian                                                                        
{"swa",-1},                    //Swahili                                                                         
{"swe",A_AUDIO_FONTTYPE_ISOIEC8859_1},                    //Swedish                                                                         
{"syc",-1},                    //Classical Syriac                                                                
{"syr",-1},                    //Syriac                                                                          
{"tah",-1},                    //Tahitian                                                                        
{"tai",-1},                    //Tai languages                                                                   
{"tam",-1},                    //Tamil                                                                           
{"tat",-1},                    //Tatar                                                                           
{"tel",-1},                    //Telugu                                                                          
{"tem",-1},                    //Timne                                                                           
{"ter",-1},                    //Tereno                                                                          
{"tet",-1},                    //Tetum                                                                           
{"tgk",-1},                    //Tajik                                                                           
{"tgl",-1},                    //Tagalog                                                                         
{"tha",-1},                    //Thai                                                                            
{"tib",-1},                    //Tibetan                                                                         
{"bod",-1},                    //                                                            
{"tig",-1},                    //Tigre                                                                           
{"tir",-1},                    //Tigrinya                                                                        
{"tiv",-1},                    //Tiv                                                                             
{"tkl",-1},                    //Tokelau                                                                         
{"tlh",-1},                    //Klingon; tlhIngan-Hol                                                           
{"tli",-1},                    //Tlingit                                                                         
{"tmh",-1},                    //Tamashek                                                                        
{"tog",-1},                    //Tonga (Nyasa)                                                                   
{"ton",-1},                    //Tonga (Tonga Islands)                                                           
{"tpi",-1},                    //Tok Pisin                                                                       
{"tsi",-1},                    //Tsimshian                                                                       
{"tsn",-1},                    //Tswana                                                                          
{"tso",-1},                    //Tsonga                                                                          
{"tuk",-1},                    //Turkmen                                                                         
{"tum",-1},                    //Tumbuka                                                                         
{"tup",-1},                    //Tupi languages                                                                  
{"tur",A_AUDIO_FONTTYPE_ISOIEC8859_3},                    //Turkish                                                                         
{"tut",-1},                    //Altaic languages                                                                
{"tvl",-1},                    //Tuvalu                                                                          
{"twi",-1},                    //Twi                                                                             
{"tyv",-1},                    //Tuvinian                                                                        
{"udm",-1},                    //Udmurt                                                                          
{"uga",-1},                    //Ugaritic                                                                        
{"uig",-1},                    //Uighur; Uyghur                                                                  
{"ukr",-1},                    //Ukrainian                                                                       
{"umb",-1},                    //Umbundu                                                                         
{"und",-1},                    //Undetermined                                                                    
{"urd",-1},                    //Urdu                                                                            
{"uzb",-1},                    //Uzbek                                                                           
{"vai",-1},                    //Vai                                                                             
{"ven",-1},                    //Venda                                                                           
{"vie",-1},                    //Vietnamese                                                                      
{"vol",-1},                    //Volap¨¹k                                                                        
{"vot",-1},                    //Votic                                                                           
{"wak",-1},                    //Wakashan languages                                                              
{"wal",-1},                    //Wolaitta; Wolaytta                                                              
{"war",-1},                    //Waray                                                                           
{"was",-1},                    //Washo                                                                           
{"wel",-1},                    //Welsh                                                                           
{"cym",-1},                    //                                                            
{"wen",A_AUDIO_FONTTYPE_ISOIEC8859_2},                    //Sorbian languages ???geuss                                                              
{"wln",-1},                    //Walloon                                                                         
{"wol",-1},                    //Wolof                                                                           
{"xal",-1},                    //Kalmyk; Oirat                                                                   
{"xho",-1},                    //Xhosa                                                                           
{"yao",-1},                    //Yao                                                                             
{"yap",-1},                    //Yapese                                                                          
{"yid",-1},                    //Yiddish                                                                         
{"yor",-1},                    //Yoruba                                                                          
{"ypk",-1},                    //Yupik languages                                                                 
{"zap",-1},                    //Zapotec                                                                         
{"zbl",-1},                    //Blissymbols; Blissymbolics; Bliss                                               
{"zen",-1},                    //Zenaga                                                                          
{"zha",-1},                    //Zhuang; Chuang                                                                  
{"chi",-1},                    //Chinese                                                                         
{"zho",-1},                    //                                                            
{"znd",-1},                    //Zande languages                                                                 
{"zul",-1},                    //Zulu                                                                            
{"zun",-1},                    //Zuni                                                                            
{"zxx",-1},                    //No linguistic content; Not applicable                                           
{"zza",-1},                    //Zaza; Dimili; Dimli; Kirdki; Kirmanjki; Zazaki 
};                                 


int GetInfo_Language_encoding(char*ulcomm)
{
   char *uLanguage = ulcomm;
   int ret = -1;
   int i;
   //strlwr(uLanguage);
   while(*uLanguage!=0)
   {
   	if((*uLanguage>64)&&(*uLanguage<91))
   	  *uLanguage +=32;
   	uLanguage++;
   }
   uLanguage = ulcomm;
   for(i=0;i<545;i++)
   {
   	 if(!strcmp(uLanguage,ISO_639_2_Code[i].language))//==
   	 	{
			ret =ISO_639_2_Code[i].coding;
   	 		break;
   		}
   }
   return ret;
}
int GetInfo_encoding_change(__audio_file_info_t *AIF)
{
	//int ret = -1;
	int i;
	unsigned char* psrc = 0;
	unsigned char* pdrc = 0;
	if(AIF->ultitleCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(AIF->ultitle + AIF->ultitle_sz-1);
		pdrc = (unsigned char*)(AIF->ultitle + 2*(AIF->ultitle_sz-1));
		for(i=0;i<AIF->ultitle_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		AIF->ultitle_sz *=2;
		AIF->ultitleCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(AIF->ulauthorCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(AIF->ulauthor + AIF->ulauthor_sz-1);
		pdrc = (unsigned char*)(AIF->ulauthor + 2*(AIF->ulauthor_sz-1));
		for(i=0;i<AIF->ulauthor_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		AIF->ulauthor_sz *=2;
		AIF->ulauthorCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(AIF->ulAlbumCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(AIF->ulAlbum + AIF->ulAlbum_sz-1);
		pdrc = (unsigned char*)(AIF->ulAlbum + 2*(AIF->ulAlbum_sz-1));
		for(i=0;i<AIF->ulAlbum_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		AIF->ulAlbum_sz *=2;
		AIF->ulAlbumCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(AIF->ulYearCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(AIF->ulYear + AIF->ulYear_sz-1);
		pdrc = (unsigned char*)(AIF->ulYear + 2*(AIF->ulYear_sz-1));
		for(i=0;i<AIF->ulYear_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		AIF->ulYear_sz *=2;
		AIF->ulYearCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	if(AIF->ulGenreCharEncode==A_AUDIO_FONTTYPE_ISOIEC8859_1)
	{
		psrc = (unsigned char*)(AIF->ulGenre + AIF->ulGenre_sz-1);
		pdrc = (unsigned char*)(AIF->ulGenre + 2*(AIF->ulGenre_sz-1));
		for(i=0;i<AIF->ulGenre_sz;i++)
		{
			if(*psrc<128)
			{
				*pdrc =*psrc;
				*(pdrc+1) = 0;
			}
			else
			{
				*(pdrc+1) = (windows1251_unicode[*psrc - 128]>>8) &0x00ff;
				*pdrc = windows1251_unicode[*psrc - 128] &0x00ff;
			}
			psrc--;
			pdrc-=2;
		}
		AIF->ulGenre_sz *=2;
		AIF->ulGenreCharEncode = A_AUDIO_FONTTYPE_UTF_16LE;
	}
	
	return 0;

}
