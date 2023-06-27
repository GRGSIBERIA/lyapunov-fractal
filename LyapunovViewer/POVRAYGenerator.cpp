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
    
    if (space.isMetal) {
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
            const auto L = std::isinf(lambda[h][w]) ? 0 : lambda[h][w];
            const auto C = color[h][w];

            const auto tx = w;
            const auto ty = L;
            const auto tz = h;

            const auto sy = L + space.thickness;

            const auto r = (float)GetRValue(C) / 255.f;
            const auto g = (float)GetGValue(C) / 255.f;
            const auto b = (float)GetBValue(C) / 255.f;

            std::string s = "";
            s += "object {\n";
            s += "    box {\n";
            s += "        <0, 0, 0>\n";
            s += "        <1, 1, 1>\n";
            s += std::format("        translate <{}, {}, {}>\n", tx, ty, tz);
            s += std::format("        scale <1, {}, 1>\n", sy);
            s += "    }\n";
            s += "    texture {\n";
            s += "        pigment {\n";
            s += std::format("            color rgb <{}, {}, {}>\n", r, g, b);
            s += "        }\n";
            s += "    }\n";

            if (space.isMetal) {
                s += "    finish {\n";
                s += "        Metal\n";
                s += "    }\n";
            }

            s += "}\n";

            ofs << s;
        }
    }
    ofs.close();

    MessageBox(NULL, TEXT("Generated POV-RAY Include File"), TEXT("INFORMATION"), MB_OK | MB_ICONINFORMATION);
}
