#pragma once
#include <functional>
#include <vector>

class TurnObserver {
public:
	virtual ~TurnObserver() = default;

	virtual void onTurnProcess() = 0;
};

class TurnManager {
private:
	std::vector<std::reference_wrapper<TurnObserver>> observers;

public:
	void addObserver(TurnObserver& obs) { observers.emplace_back(obs); }

	void notifyTurn() {
		for (const auto& obs : observers) {
			obs.get().onTurnProcess();
		}
	}
};