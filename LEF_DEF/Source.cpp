
#include <vector>
#include <string>
#include <map>
#include <utility>

using namespace std;

//������ LEF, ������

class SITE {
    string Site[3]; //  �������� �� ������������, �������� ���������� �� 7
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
    enum class MacroClass { COVER , RING , BLOCK, PAD , CORE , ENDCAP };//�������� �������
    class FOREIGN {
        string foreignCellName;
        float F[2];
       
    };
    float ORIGIN[2];
    float SIZE[2];
    string SYMMETRY[3];//�������� ����������� ��������(�������� ������ �� ������)
    vector<LAYER> DENSITY_Layer;  // DENSITY
    vector<SITE> Site_vec;
};
class LEF_File {
    char* BUSBITCHARS = new char[2]; // ����������� ������� ��� ������ �����(����� �������� �� 2 ��� 3) g�� ��������� []
    string version;
    char DIVIDERCHAR = " / ";  //������ ����������� �� ��������� /
    vector<Macro> Macro_vec;
};

// ������ LEF,�����
    // ���������� ����������
    enum class Orientation {
        N,    // 0�
        S,    // 180�
        E,    // 90�
        W,    // 270�
        FN,   // Flipped N (mirror Y)
        FS,   // Flipped S (mirror Y)
        FE,   // Flipped E (mirror X)
        FW    // Flipped W (mirror X)
    };

    // ������� �� ���������
    struct Position {
        int x;          // ���������� X
        int y;          // ���������� Y
        Orientation orientation; // ����������

        Position(int x = 0, int y = 0, Orientation orient = Orientation::N)
            : x(x), y(y), orientation(orient) {}
    };

    

void main() {}