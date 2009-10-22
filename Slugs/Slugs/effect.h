#pragma once

/*
	class Effect
	Base class fo all effects handled by the FXManager
*/

class Effect
{

protected:

	bool continuous;
	float life;

public:

	Effect();
	~Effect();

	virtual void Update(float elaspedTime) {};
	virtual void Render() {};

	bool Continuous();
	float Life();

	void SetContinuous(bool isContinuous);
	void SetLife(float newLife);

};