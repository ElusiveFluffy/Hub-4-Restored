#pragma once
namespace PluginCore
{
	bool Setup();
	void OnTyInitialized();
	void Tick(float deltaSeconds);
	bool ImGuiWantCaptureMouse();
	bool WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

