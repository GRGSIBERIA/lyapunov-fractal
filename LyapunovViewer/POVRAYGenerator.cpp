#include "POVRAYGenerator.h"
#include <format>

POVRAYGenerator::POVRAYGenerator(HWND& hWnd, const V2D& lambda, const C2D color, const float width2d, const float height2d, const Space& space)
{
	const auto fname = OpenSaveDialog(hWnd, TEXT("POV-RAY include file {*.inc}\0*.inc\0") TEXT("All files {*.*}\0*.*\0\0"));

	std::string output = "";
	std::string temp = std::string("") +
        "object {\n" +
        "    box {\n" +
        "        <0, 0, 0>\n" +
        "        <1, 1, 1>\n" +
        "        translate <{}, {}, {}>\n" +
        "        scale <1, {}, 1>\n" +
        "    }\n" +
        "    texture {\n" +
        "        pigment {\n" +
        "            color rgb <{}, {}, {}>" +
        "        }" +
        "    }\n"+
        "    {}\n"+
        "}\n";

    std::string finish = "";
    
    if (true) {
        finish = std::string("") + 
            "    finish {\n"+
            "        Metal\n"+
            "    }";
    }

    if (fname.size() <= 0) return;

    std::ofstream ofs(fname, std::ios::trunc | std::ios::out);

    for (int h = 0; h < height2d; ++h) {
        for (int w = 0; w < width2d; ++w) {
            // lambdaで位置とスケールを合わせる
            // colorでpigmentの色を変える
            const auto L = lambda[h][w];
            const auto C = color[h][w];

            const auto tx = w;
            const auto ty = L;
            const auto tz = h;

            const auto sy = L + space.thickness;

            const auto r = GetRValue(C);
            const auto g = GetGValue(C);
            const auto b = GetBValue(C);

            ofs << std::format(temp, tx, ty, tz, sy, r, g, b, finish);
        }
    }
    ofs.close();
}
