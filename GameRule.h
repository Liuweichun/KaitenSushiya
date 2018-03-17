#pragma once
#include "Scene.h"

//ゲームルールクラス
class GameRule : public Scene 
{
protected:
	//ゲーム情報
	DI::Mouse mouse;

	//構造体型定義
	struct Object {
		bool active;			//生存判断
	};
	Object haikei;

public:
	explicit GameRule() {}
	virtual ~GameRule() {}

	virtual void InitializeResource() override;
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void Finalize() override;
};