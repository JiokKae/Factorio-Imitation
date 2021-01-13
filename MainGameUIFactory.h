#pragma once

class UI;
class MainGameUIFactory
{
public:
	enum UIEnum {
		CHARACTER_UI,
		BURNER_MINING_DRILL_UI,
		STONE_FURNACE_UI,
		END
	};

public:
	static UI* CreateUI(UIEnum uiId);
};

