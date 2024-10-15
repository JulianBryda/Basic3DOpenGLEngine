#include "ShaderVarNode.hpp"
#include "ShaderVarNode.hpp"
#include <Windows.h>

void ShaderVarNode::openFileDialog(char* buffer)
{
	OPENFILENAMEA ofn;
	char szFile[_MAX_PATH] = { 0 };

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFilter = "All\0*.*\0PNG\0*.png\0JPG\0*.jpg\0";
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn))
	{
		strcpy(buffer, ofn.lpstrFile);
	}
}