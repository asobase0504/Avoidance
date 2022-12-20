#include "load_stage.h"
#include "stage.h"
#include "file.h"
#include "objectX.h"

#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>

CStage* LoadAll(const wchar_t* filename, const D3DXVECTOR3& inPos)
{
	CStage* stage = CStage::Create();

	nlohmann::json list = LoadJson(filename);

	{// è∞
		D3DXVECTOR3 pos = D3DXVECTOR3(list["FLOOR"]["POS"][0], list["FLOOR"]["POS"][1], list["FLOOR"]["POS"][2]) + inPos;
		D3DXVECTOR3 rot = D3DXVECTOR3(list["FLOOR"]["ROT"][0], list["FLOOR"]["ROT"][1], list["FLOOR"]["ROT"][2]);
		D3DXVECTOR3 scale = D3DXVECTOR3(list["FLOOR"]["SCALE"][0], list["FLOOR"]["SCALE"][1], list["FLOOR"]["SCALE"][2]);

		stage->SetFloor(pos, rot, scale);
	}

	{// ï«
		int size = (int)list["WALL"].size();

		for (int i = 0; i < size; ++i)
		{

			D3DXVECTOR3 pos = D3DXVECTOR3(list["WALL"][i]["POS"][0], list["WALL"][i]["POS"][1], list["WALL"][i]["POS"][2]) + inPos;
			D3DXVECTOR3 rot = D3DXVECTOR3(list["WALL"][i]["ROT"][0], list["WALL"][i]["ROT"][1], list["WALL"][i]["ROT"][2]);
			D3DXVECTOR3 scale = D3DXVECTOR3(list["WALL"][i]["SCALE"][0], list["WALL"][i]["SCALE"][1], list["WALL"][i]["SCALE"][2]);
			stage->SetWall(i,pos, rot, scale);
		}
	}

	{// ãÛíÜè∞ÇÃçÏê¨
		int size = (int)list["BLOCK"].size();

		for (int i = 0; i < size; ++i)
		{

			D3DXVECTOR3 pos = D3DXVECTOR3(list["BLOCK"][i]["POS"][0], list["BLOCK"][i]["POS"][1], list["BLOCK"][i]["POS"][2]) + inPos;
			D3DXVECTOR3 rot = D3DXVECTOR3(list["BLOCK"][i]["ROT"][0], list["BLOCK"][i]["ROT"][1], list["BLOCK"][i]["ROT"][2]);
			D3DXVECTOR3 scale = D3DXVECTOR3(list["BLOCK"][i]["SCALE"][0], list["BLOCK"][i]["SCALE"][1], list["BLOCK"][i]["SCALE"][2]);
			stage->AddFloor(pos, rot, scale);
		}
	}

	{// ìGÇÃçÏê¨
		int size = (int)list["ENEMY"].size();

		for (int i = 0; i < size; ++i)
		{
			int type = list["ENEMY"][i]["TYPE"];
			D3DXVECTOR3 pos = D3DXVECTOR3(list["ENEMY"][i]["POS"][0], list["ENEMY"][i]["POS"][1], list["ENEMY"][i]["POS"][2]) + inPos;
			D3DXVECTOR3 rot = D3DXVECTOR3(list["ENEMY"][i]["ROT"][0], list["ENEMY"][i]["ROT"][1], list["ENEMY"][i]["ROT"][2]);
			int fream = list["ENEMY"][i]["POP_FREAM"];
			stage->AddEnemy(type, pos, rot, fream);
		}
	}

	return stage;
}

void SaveAll(CStage* inStage, const std::string& filename)
{
	//D3DXVECTOR3Çvector<float>Ç…ïœçXÇ∑ÇÈ
	auto Vector3ToVectorFloat = [](D3DXVECTOR3 inVector)->std::vector<float>
	{
		return{ inVector.x, inVector.y, inVector.z };
	};

	CStage* stage = inStage;
	nlohmann::json saveJson;

	{// è∞
		saveJson["FLOOR"]["POS"] = Vector3ToVectorFloat(stage->GetFloor()->GetPos());
		saveJson["FLOOR"]["ROT"] = Vector3ToVectorFloat(stage->GetFloor()->GetRot());
		saveJson["FLOOR"]["SCALE"] = Vector3ToVectorFloat(stage->GetFloor()->GetScale());
	}

	{// ï«
		int size = inStage->GetWall().size();

		for (int i = 0; i < size; ++i)
		{
			saveJson["WALL"][i]["POS"] = Vector3ToVectorFloat(stage->GetWall()[i]->GetPos());
			saveJson["WALL"][i]["ROT"] = Vector3ToVectorFloat(stage->GetWall()[i]->GetRot());
			saveJson["WALL"][i]["SCALE"] = Vector3ToVectorFloat(stage->GetWall()[i]->GetScale());
		}
	}

	{// ãÛíÜè∞ÇÃçÏê¨
		int size = inStage->GetMidairFloor().size();

		for (int i = 0; i < size; ++i)
		{
			saveJson["BLOCK"][i]["POS"] = Vector3ToVectorFloat(stage->GetMidairFloor()[i]->GetPos());
			saveJson["BLOCK"][i]["ROT"] = Vector3ToVectorFloat(stage->GetMidairFloor()[i]->GetRot());
			saveJson["BLOCK"][i]["SCALE"] = Vector3ToVectorFloat(stage->GetMidairFloor()[i]->GetScale());
		}
	}

	{// ìGÇÃçÏê¨
		int size = inStage->GetEnemy().size();

		for (int i = 0; i < size; ++i)
		{
			saveJson["ENEMY"][i]["TYPE"] = stage->GetEnemy()[i].type;
			saveJson["ENEMY"][i]["POS"] = Vector3ToVectorFloat(stage->GetEnemy()[i].pos);
			saveJson["ENEMY"][i]["ROT"] = Vector3ToVectorFloat(stage->GetEnemy()[i].rot);
			saveJson["ENEMY"][i]["POP_FREAM"] = stage->GetEnemy()[i].popFream;
		}
	}

	SaveJson(saveJson,filename);
}