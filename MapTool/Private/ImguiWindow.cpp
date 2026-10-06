#include "stdafx.h"
#include "ImguiWindow.h"
#include "Client_Defines.h"

#include"imgui.h"



CImguiWindow::CImguiWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice(pDevice), m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CImguiWindow::Initialize()
{
    if (nullptr == m_pDevice || nullptr == m_pContext)
    {
        MSG_BOX("No Existance Device orContext");
        return E_FAIL;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiIO& IO = ImGui::GetIO(); (void)IO;
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Gamepad Controls
    IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;


    // 1280, 720 사이즈로 설정
    IO.DisplaySize = ImVec2{g_iWinSizeX, g_iWinSizeY};
    ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.f;
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::GetStyle().WindowRounding = 0.f;
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.f;

    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);    
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);
    Load_Font();
    return S_OK;
}

HRESULT CImguiWindow::Start_Render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    
    // demo window를 보고 싶다면 아래의 주석을 풀어보십시오..
    ImGui::ShowDemoWindow(); // Show demo window! :)
    {

        static float f = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, Worldd");
        // Create a window called "Hello, world!" and append into it.
        // Display some text (you can use a format strings too) // Edit bools storing our window open/close state
        //ImGui::Text("This is some useful text."); ImGui::Checkbox("Demo Window", &show_demo_window); ImGui::Checkbox("Another Window", &show_another_window);
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        // Edit 1 float using a slider from 0.0f to 1.0f
       // ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
       // if (ImGul::Button("Button"))
       //     counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        // Buttons return true when clicked (most widgets return true when edited/activated)
        //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.Of/io.Framerate, io.Framerate);
        ImGui::End();
    }

    return S_OK;
}

HRESULT CImguiWindow::End_Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();


    return S_OK;
}

void CImguiWindow::Load_Font()
{
    ImGuiIO& io = ImGui::GetIO();
    ImFont* font = nullptr;

    std::string font_file = "../TTF/malgun.ttf";

    std::ifstream ifile;
    ifile.open(font_file);

    if (ifile)
    {
        font = io.Fonts->AddFontFromFileTTF(font_file.c_str(), 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    }
    else
    {
        font = io.Fonts->AddFontDefault();
    }
    IM_ASSERT(font != NULL);
}

CImguiWindow* CImguiWindow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

    CImguiWindow* pInstance = new CImguiWindow(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CImguiWindow");
        Safe_Release(pInstance);
    }

    return pInstance;

}

void CImguiWindow::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}
