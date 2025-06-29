
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <fstream>
#include <map>
#include <sstream>
#include <algorithm>
#include <memory>

using namespace std;

//Чтение DEF, начало
 // Ориентация компонента
enum Orientation {
    N,    // 0°
    S,    // 180°
    E,    // 90°
    W,    // 270°
    FN,   // Flipped N (mirror Y)
    FS,   // Flipped S (mirror Y)
    FE,   // Flipped E (mirror X)
    FW    // Flipped W (mirror X)
};

struct Rect {
    double x1, y1, x2, y2;

    Rect(double _x1 = 0, double _y1 = 0, double _x2 = 0, double _y2 = 0)
        : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

// Позиция на плоскости
struct Position {
    int x;          // Координата X
    int y;          // Координата Y
    enum Orientation orientation; // Ориентация

    Position(int x = 0, int y = 0, Orientation orient = Orientation::N)
        : x(x), y(y), orientation(orient) {}
};

enum SOURCE_class {
    NETLIST = 0,
    DIST = 1,
    USER = 2,
    TIMING = 3
};
enum FIXED_class {
    FIXED = 0,
    COVER = 1,
    PLACED = 2,
    UNPLACED = 3
};
class COMPONENTS_clas {
public:
    string compName;
    string modelName;
    //string EQMASTER;
    Position POS;
    enum SOURCE_class SOURCE;
    enum FIXED_class FIXED;
    //int HALO;
    //string PROPERTY;
    // string REGION;
    //int ROUTEHALO;
    //string minLayer;
    //string maxLayer;
    //int WEIGHT;
    COMPONENTS_clas (string _compName, string _modelName) : compName(_compName), modelName(_modelName) {}
};
class BLOCKAGES_class {
    string layerName;
    //Вернусь позже стр 242
    vector<Rect> RECT;
    int SPACING = 0;
    int DESIGNRULEWIDTH = 0;
    string COMPONENT_compName;
    string FILLS;
    string SLOTS;
    string PUSHDOWN;
    string EXCEPTPGNET;
    vector<Rect> POLYGON;
    string SOFT;///Вот здесь надо подумать над перечислениями с 243
    string PARTIAL;
    string COMPONENT;
};
class VIAS_class {


};
enum DIRECTION_class {
    INPUT,
    OUTPUT,
    INOUT,
    FEEDTHRU
};
enum USE_class {
    SIGNAL,
    POWER,
    GROUND,
    CLOCK,
    TIEOFF,
    ANALOG,
    SCAN,
    RESET
};
class VIA_class {
public:
    string viaName;
    int x;
    int y;
};
class LAYER_class {
public:
    string layerName;
    Rect rect;
};

class PINS_class {
public:
    string pinName;
    string netName;
       //SPECIAL
    enum DIRECTION_class DIRECTION;
    //NETEXPR
    //SUPPLYSENSITIVITY powerPinName
    //  GROUNDSENSITIVITY groundPinName
    enum USE_class USE;
    vector<LAYER_class>  LAYER;
    VIA_class VIA;
    enum FIXED_class PLACED_PIN;
    Position POS;
    PINS_class(string _compName, string _modelName) : pinName(_compName), netName(_modelName) {}
};
class NETS_class {


};
class SPECIALNETS_class {


};

class DEF_File {
public:
    char* BUSBITCHARS = new char[2]; // Разделяющие символы для шинных битов(нужно уточнить их 2 или 3) gпо умолчанию []
    float version;
    char DIVIDERCHAR = '/';  //символ разделитель по умолчанию /
    string DESIGN;
    int COUNT_COMPONENTS = 0;
    vector<COMPONENTS_clas> COMPONENTS;
    int numBlockages;
    vector<BLOCKAGES_class> BLOCKAGES;
    int COUNT_VIAS = 0;
    int COUNT_PINS = 0;
    vector<PINS_class> PINS;
    int COUNT_NETS = 0;
    int COUNT_SPECIALNETS = 0;
    
    void push_back_COMPONENTS(string a,string b) {
        COMPONENTS_clas buffer_components(a, b);
        COMPONENTS.push_back(buffer_components);
    }
    void push_back_PINS(string a, string b) {
        PINS_class buffer_pins(a,b);
        PINS.push_back(buffer_pins);
    }
};

void parsComponents(ifstream* inFile, DEF_File* def, int* i) {
    string buffer;
    vector<string> split_buffer;
   // cout << "START COMPONENTS " << *i << endl;
    
    while (getline(*inFile, buffer)) {
        *i = *i + 1;
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));
        if (buffer == "END COMPONENTS") {       
           // cout << "END COMPONENTS " << *i << endl;          
            return;
        }
        
        def->push_back_COMPONENTS(split_buffer[1], split_buffer[2]);
            for (const auto& word : split_buffer) {
                if (word == "PLACED") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].FIXED = PLACED;
                }
                if (word == "COVER") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].FIXED = COVER;
                }
                if (word == "UNPLACED") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].FIXED = UNPLACED;
                }
                if (word == "FIXED") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].FIXED = FIXED;
                }
                if (word == "NETLIST") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].SOURCE = NETLIST;
                }
                if (word == "USER") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].SOURCE = USER;
                }
                if (word == "DIST") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].SOURCE = DIST;
                }
                if (word == "TIMING") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].SOURCE = TIMING;
                }
                if (word == "(") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.x = stoi(*(& word + 1));
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.y = stoi(*(&word + 2));
                   //  ;
                }
                if (word == "N") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = N;
                }
                if (word == "S") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = S;
                }
                if (word == "E") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = E;
                }
                if (word == "W") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = W;
                }
                if (word == "FN") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = FN;
                }
                if (word == "FS") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = FS;
                }
                if (word == "FE") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = FE;
                }
                if (word == "FW") {
                    def->COMPONENTS[def->COMPONENTS.size() - 1].POS.orientation = FW;
                }

               
            }
 
        split_buffer.clear();
    }
}
void parsNets(ifstream* inFile, DEF_File* def, int* i) {
    string buffer;
    vector<string> split_buffer;
   // cout << "START Nets " << *i << endl;

    while (getline(*inFile, buffer)) {
        *i = *i + 1;
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));
        if (buffer == "END NETS") {
            //cout << "END Nets " << *i << endl;
            return;
        }



        split_buffer.clear();
    }

}
void parsSpecialnets(ifstream* inFile, DEF_File* def, int* i) {
    string buffer;
    vector<string> split_buffer;
   // cout << "START SPECIALNETS " << *i << endl;

    while (getline(*inFile, buffer)) {
        *i = *i + 1;
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));
        if (buffer == "END SPECIALNETS") {
           // cout << "END SPECIALNETS " << *i << endl;
            return;
        }



        split_buffer.clear();
    }

}
void parsPins(ifstream* inFile, DEF_File* def, int* i) {
    string buffer;
    string buffer_end;
    vector<string> split_buffer;
    cout << "START PINS " << *i << endl;

    while (getline(*inFile, buffer)) {
        *i = *i + 1;
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));
        if (buffer == "END PINS") {
            cout << "END PINS " << *i << endl;
            return;
        }
        if (split_buffer[0] == "-") {
            def->push_back_PINS(split_buffer[1], split_buffer[4]);
        }
        
        for (const auto& word : split_buffer) {
            if (word == "DIRECTION") {
                if ( *( & word + 1) == "INPUT") {
                    def->PINS[def->PINS.size() - 1].DIRECTION = INPUT;
                }
                if (*(&word + 1) == "OUTPUT") {
                    def->PINS[def->PINS.size() - 1].DIRECTION = OUTPUT;
                }
                if (*(&word + 1) == "INOUT") {
                    def->PINS[def->PINS.size() - 1].DIRECTION = INOUT;
                }
                if (*(&word + 1) == "FEEDTHRU") {
                    def->PINS[def->PINS.size() - 1].DIRECTION = FEEDTHRU;
                }
            }
            
            if (word == "USE") {
                if (*(&word + 1) == "SIGNAL") {
                    def->PINS[def->PINS.size() - 1].USE = SIGNAL;
                }
                if (*(&word + 1) == "POWER") {
                    def->PINS[def->PINS.size() - 1].USE = POWER;
                }
                if (*(&word + 1) == "GROUND") {
                    def->PINS[def->PINS.size() - 1].USE = GROUND;
                }
                if (*(&word + 1) == "CLOCK") {
                    def->PINS[def->PINS.size() - 1].USE = CLOCK;
                }
                if (*(&word + 1) == "TIEOFF") {
                    def->PINS[def->PINS.size() - 1].USE = TIEOFF;
                }
                if (*(&word + 1) == "ANALOG") {
                    def->PINS[def->PINS.size() - 1].USE = ANALOG;
                }
                if (*(&word + 1) == "SCAN") {
                    def->PINS[def->PINS.size() - 1].USE = SCAN;
                }
                if (*(&word + 1) == "RESET") {
                    def->PINS[def->PINS.size() - 1].USE = RESET;
                }
            }
            if (word == "LAYER") {
                LAYER_class o;
                def->PINS[def->PINS.size() - 1].LAYER.push_back(o);
                def->PINS[def->PINS.size() - 1].LAYER[def->PINS[def->PINS.size() - 1].LAYER.size() - 1].layerName = (*(&word + 1));
                def->PINS[def->PINS.size() - 1].LAYER[def->PINS[def->PINS.size() - 1].LAYER.size() - 1].rect.x1 = stoi(*(&word + 3));
                def->PINS[def->PINS.size() - 1].LAYER[def->PINS[def->PINS.size() - 1].LAYER.size() - 1].rect.x2 = stoi(*(&word + 4));
                def->PINS[def->PINS.size() - 1].LAYER[def->PINS[def->PINS.size() - 1].LAYER.size() - 1].rect.y1 = stoi(*(&word + 7));
                def->PINS[def->PINS.size() - 1].LAYER[def->PINS[def->PINS.size() - 1].LAYER.size() - 1].rect.y2 = stoi(*(&word + 8));
                           
            }
            
        }
      

        split_buffer.clear();
    }

}
void parsVias(ifstream* inFile, DEF_File* def, int* i) {
    string buffer;
    vector<string> split_buffer;
   // cout << "START VIAS " << *i << endl;
   
    while (getline(*inFile, buffer)) {
        *i = *i + 1;
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));
        if (buffer == "END VIAS") {            
            //cout << "END VIAS " << *i << endl;         
            return;
        }

       
        split_buffer.clear();
    }

}

// Чтение DEF,конец
   

    

void main() {
    string buffer;
    vector<string> split_buffer;
    string BUS_DIV;
    DEF_File* def;
    def = new DEF_File;
    ifstream inFile("C:\\c880.def");
    if (!inFile.is_open()) {
        throw std::runtime_error("Could not open file: " );
    }
    int i = 0;

    while (getline(inFile, buffer)) {
        boost::trim(buffer);
        boost::split(split_buffer, buffer, boost::is_any_of(" "));

        i++;
        if (split_buffer.front() == "VERSION") {
            def->version = stof(split_buffer[1]);
        }
        if (split_buffer.front() == "DIVIDERCHAR") {
            BUS_DIV = split_buffer[1];
            def->DIVIDERCHAR = BUS_DIV[1];
        }
        if (split_buffer.front() == "BUSBITCHARS") {
            BUS_DIV = split_buffer[1];
            def->BUSBITCHARS[0] = BUS_DIV[1];
            def->BUSBITCHARS[1] = BUS_DIV[2];
        }
        if (split_buffer.front() == "DESIGN") {
            def->DESIGN = split_buffer[1];
        }
        if (split_buffer.front() == "COMPONENTS") {
            def->COUNT_COMPONENTS = stoi(split_buffer[1]);
            parsComponents(&inFile , def, &i);
        }
        if (split_buffer.front() == "VIAS") {
            def->COUNT_VIAS = stoi(split_buffer[1]);
            parsVias(&inFile, def, &i);
        }
        if (split_buffer.front() == "PINS") {
            def->COUNT_PINS = stoi(split_buffer[1]);
            parsPins(&inFile, def, &i);
        }
        if (split_buffer.front() == "NETS") {
            def->COUNT_NETS = stoi(split_buffer[1]);
            parsNets(&inFile, def, &i);
        }
        if (split_buffer.front() == "SPECIALNETS") {
            def->COUNT_SPECIALNETS = stoi(split_buffer[1]);
            parsSpecialnets(&inFile, def, &i);
        }

        
      
        if (i>1000) break;
        split_buffer.clear();
    }
    cout << def->COMPONENTS[933].POS.orientation << " l " << def->COMPONENTS[933].modelName << " l " << def->COMPONENTS[933].compName << " l " << def->COMPONENTS[933].SOURCE << " l "<< def->COMPONENTS[933].FIXED << " l " << def->COMPONENTS[933].POS.x << " l " << def->COMPONENTS[933].POS.y << " l " << endl;
    cout << def->PINS[87].LAYER[1].layerName << " l " << def->PINS[87].LAYER[1].rect.y2 << endl;
}

