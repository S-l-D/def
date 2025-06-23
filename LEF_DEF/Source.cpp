
#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

//Чтение LEF, начало

class SITE {
    string Site[3]; //  Почитать по внимательнее, возможно расширение до 7
};
class RECT {
    float Rect[5];
};

class LAYER {
    string layerName;
    vector<RECT> DENSITY_Rect;
};

class Macro {
    string macroName;
    enum class MacroClass { COVER , RING , BLOCK, PAD , CORE , ENDCAP };//Уточнить подтипы
    class FOREIGN {
        string foreignCellName;
        float F[2];
       
    };
    float ORIGIN[2];
    float SIZE[2];
    string SYMMETRY[3];//Уточнить колличество символов(возможна замена на вектор)
    vector<LAYER> DENSITY_Layer;  // DENSITY
    vector<SITE> Site_vec;
};
class LEF_File {
    char* BUSBITCHARS = new char[2]; // Разделяющие символы для шинных битов(нужно уточнить их 2 или 3) gпо умолчанию []
    string version;
    char DIVIDERCHAR = " / ";  //символ разделитель по умолчанию /
    vector<Macro> Macro_vec;
};

// Чтение LEF,конец
    // Ориентация компонента
    enum class Orientation {
        N,    // 0°
        S,    // 180°
        E,    // 90°
        W,    // 270°
        FN,   // Flipped N (mirror Y)
        FS,   // Flipped S (mirror Y)
        FE,   // Flipped E (mirror X)
        FW    // Flipped W (mirror X)
    };

    // Позиция на плоскости
    struct Position {
        int x;          // Координата X
        int y;          // Координата Y
        Orientation orientation; // Ориентация

        Position(int x = 0, int y = 0, Orientation orient = Orientation::N)
            : x(x), y(y), orientation(orient) {}
    };

    

void main() {}