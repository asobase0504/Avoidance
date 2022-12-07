//=============================================================================
// 
// �F�Ǘ��N���X
// Author YudaKaito
// 
//=============================================================================
#ifndef _THEME_COLOR_H_
#define _THEME_COLOR_H_

#include <d3dx9.h>
#include <map>
#include <string>

//-----------------------------------------------------------------------------
// �F�Ǘ��N���X
//-----------------------------------------------------------------------------
class CColor
{
public:	// �񋓌^
	enum EColorConfig
	{
		COLOR_0,
		COLOR_1,
		COLOR_2,
		COLOR_3,
		COLOR_MAX,
	};

public:	// �����o�[�֐�
	CColor();
	~CColor();

	HRESULT Init();
	void Uninit();

	// Getter
	const D3DXCOLOR& GetColor(EColorConfig inConfig);
	int GetSize() { return typeNumber; }

	// Setter
	void SetTheme(int idx);

private: // ���I�����o�[�ϐ�
	std::map<int, D3DXCOLOR> m_color;
	int typeNumber;
};

#endif // !_COLOR_H_
