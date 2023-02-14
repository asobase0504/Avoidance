//=============================================================================
//
// ファイル
// Author : Hamada Ryuuga
// Author : Yuda Kaito
//
//=============================================================================
//-----------------------------------------------------------------------------
// include
//-----------------------------------------------------------------------------
#include "file.h"
#include "letter.h"
#include "application.h"

//=============================================================================
// 読込み
// Author : Hamada Ryuuga
// Author : Yuda Kaito
//=============================================================================
nlohmann::json LoadJson(const wchar_t* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		nlohmann::json list;	// リストの作成

		ifs >> list;
		return list;
	}

	/* ↓ファイルを開くのを失敗した場合↓ */

	return nullptr;
}

//=============================================================================
// 読込み
// Author : Yuda Kaito
//=============================================================================
nlohmann::json LoadJson(const char* cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		nlohmann::json list;	// リストの作成

		ifs >> list;
		return list;
	}

	/* ↓ファイルを開くのを失敗した場合↓ */

	return nullptr;
}

//=============================================================================
// 読込み
// Author : Yuda Kaito
//=============================================================================
nlohmann::json LoadJson(std::string cUrl)
{
	std::ifstream ifs(cUrl);

	if (ifs)
	{
		nlohmann::json list;	// リストの作成

		ifs >> list;
		return list;
	}

	/* ↓ファイルを開くのを失敗した場合↓ */

	return nullptr;
}

//=============================================================================
// 値の読込み
// Author : Yuda Kaito
//=============================================================================
void LoadValueBundledData(nlohmann::json* inJson, DeepTable& inTable)
{
	std::vector<ConvTable> table = inTable.table;

	for (int i = 0; i < (int)table.size(); i++)
	{
		if ((*inJson).count(table[i].name) == 0)
		{ // タグがなかった場合
			continue;
		}

		/* ↓タグがあった場合↓ */

		switch (table[i].type)
		{
		case UseChack:	// 構造体
			if (((DeepTable*)table[i].store)->use != nullptr)
			{
				*((DeepTable*)table[i].store)->use = true;
			}
			// 再起処理
			LoadValueBundledData(&((*inJson)[table[i].name]), *((DeepTable*)table[i].store));
			break;
		case Vector3:	// D3DXVECTOR3型
		{
			auto VectorFloatToVector3 = [](std::vector<float> inVector)->D3DXVECTOR3
			{
				return D3DXVECTOR3(inVector[0], inVector[1], inVector[2]);
			};

			*((D3DXVECTOR3*)table[i].store) = VectorFloatToVector3((*inJson)[table[i].name]);
		}
		break;
		case SColor:	// D3DXCOLOR型
		{
			auto VectorFloatToColor = [](std::vector<float> inVector)->D3DXCOLOR
			{
				return D3DXCOLOR(inVector[0], inVector[1], inVector[2], inVector[3]);
			};

			*((D3DXCOLOR*)table[i].store) = VectorFloatToColor((*inJson)[table[i].name]);
		}
		break;
		case Float:	// float型
			*((float*)table[i].store) = (*inJson)[table[i].name];
			break;
		case Int:	// Int型
			*((int*)table[i].store) = (*inJson)[table[i].name];
			break;
		case Bool:	// bool型
			*((bool*)table[i].store) = (*inJson)[table[i].name];
			break;

		default:
			break;
		}
	}
}

//=============================================================================
// Jsonファイルでの書込み
// Author : Hamada Ryuuga
// Author : Yuda Kaito
//=============================================================================
void SaveJson(nlohmann::json inJson, const std::string cUrl)
{
	auto jobj = inJson.dump();
	std::ofstream writing_file;
	const std::string pathToJSON = cUrl;
	writing_file.open(pathToJSON, std::ios::out);
	writing_file << jobj << std::endl;
	writing_file.close();
}
