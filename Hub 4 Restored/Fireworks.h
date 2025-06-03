#pragma once
#include "StaticProp.h"

struct FireworksMKProp : StaticProp {
	void Message(MKMessage* pMsg);
	void Update();
};

namespace Fireworks {
	inline StaticPropDesc FireworksDesc{};
	void InitGameObject(KromeIni* globalModel);
}
