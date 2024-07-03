#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <iostream>
#include "event.hpp"

using namespace std;

struct MouseEventArgs : public EventArgs {
	MouseEventArgs(double x, double y)
		: x(x), y(y) {}

	double x, y;
	// you can add wheel or buttons etc...
};

typedef Event<EventSender&, MouseEventArgs&> MouseEventHandler;

class Mouse : public EventSender {
public:
	Mouse() {
		GetCursorPos(&pos_before);
	}

	void update() {
		POINT pos;
		GetCursorPos(&pos);

		if (pos.x != pos_before.x || pos.y != pos_before.y) {
			MouseEventArgs args = { (double)pos.x, (double)pos.y };
			if (Event_Move) Event_Move.invoke(*this, args);
			pos_before = pos;
		}
	}

	MouseEventHandler Event_Move;
	// MouseEventHandler Event_Wheel; // you can make whatever you want
	// MouseEventHandler Event_Button;

private:
	POINT pos_before;
};

class Application {
public:
	Application() :
		exit(false) {
		mouse.Event_Move += make_delegate(this, &Application::OnMouseMove);
		mouse.Event_Move += make_delegate(this, &Application::CheckExit);
	}

	bool update() {
		if (exit) return false;
		mouse.update();
		// keyboard.update();
		// ...
		return true;
	}

private:
	void OnMouseMove(EventSender& sender, MouseEventArgs& args) {
		cout << "Mouse Moved! x=" << args.x << ", y=" << args.y << "\n";
	}

	void CheckExit(EventSender& sender, MouseEventArgs& args) {
		if (args.x == 0 && args.y == 0) exit = true;
	}

	bool exit;
	Mouse mouse;
};

int main() {
	cout << "mouse event demo, place cursor x=0, y=0 to exit\n";
	Application app;
	Sleep(10000);
	while (app.update())
		Sleep(10);

	return 0;
}