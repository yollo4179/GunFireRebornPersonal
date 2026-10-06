# 건파이어 리본 포팅 가이드

## 1. 준비 환경

| 항목 | 프로젝트 기준 |
| --- | --- |
| OS | Windows x64 |
| C++ 빌드 도구 | Visual Studio, MSVC v143, Windows SDK 10 계열 |
| 그래픽 API | DirectX 11 |
| 모델 SDK | Autodesk FBX SDK 2020.3.7 |
| 외부 라이브러리 | DirectXTK, Effects11, Assimp, ImGui, FMOD |
| 형상 관리 | Git, Git LFS |

Visual Studio Installer에서 **C++를 사용한 데스크톱 개발**과 v143 도구 집합을 설치한다. 엔진 프로젝트는 FBX SDK 헤더를 `C:\Program Files\Autodesk\FBX\FBX SDK\2020.3.7\include`에서 찾는다. 다른 위치에 설치했다면 `Engine/Default/Engine.vcxproj`의 추가 포함·라이브러리 디렉터리를 설치 위치에 맞춘다.

## 2. 소스와 리소스 복원

```powershell
git lfs install
git clone https://github.com/yollo4179/GunFireRebornPersonal.git
Set-Location GunFireRebornPersonal
git lfs pull
git lfs fsck
```

저장소 루트가 원본 프로젝트의 `Framework` 폴더에 해당한다. `Framework.sln`과 `Client`, `Engine`, `EngineSDK`, `MapTool`, `AnimTool`의 상대 위치를 유지한다. `Bin`에는 실행 리소스도 있으므로 폴더 전체를 지우면 안 된다.

## 3. 엔진 → 게임·도구 빌드

`Framework.sln`을 열고 **Debug | x64**를 선택한다. `Engine`을 먼저 빌드한 뒤 루트에서 `UpdateLib.bat`를 실행해 DLL·LIB·헤더·셰이더를 복사한다. 이후 `Client`, `MapTool`, `AnimTool`을 빌드한다.

Developer PowerShell 명령은 다음과 같다.

```powershell
msbuild .\Engine\Default\Engine.vcxproj /m /p:Configuration=Debug /p:Platform=x64
.\UpdateLib.bat
msbuild .\Client\Default\Client.vcxproj /m /p:Configuration=Debug /p:Platform=x64
msbuild .\MapTool\Default\MapTool.vcxproj /m /p:Configuration=Debug /p:Platform=x64
msbuild .\AnimTool\Default\AnimTool.vcxproj /m /p:Configuration=Debug /p:Platform=x64
```

각 명령이 성공했는지 확인한 후 다음 명령을 실행한다. 엔진은 `Engine/Bin`, 게임은 `Client/Bin`, 도구는 각 `MapTool/Bin`, `AnimTool/Bin`에 출력된다. 게임만 실행할 때는 도구 빌드가 필수는 아니다.

링크에는 `Engine.lib`, `ImguiDX11.lib`, `DirectXTKd.lib`, `Effects11d.lib`, `assimp-vc143-mtd.lib`, `fmodL_vc.lib` 등이 사용된다. `.lib`는 `Engine/ThirdPartyLib`, `EngineSDK/Lib`, 도구의 `ThirdPartyLib`에서 확인한다. Debug/Release 라이브러리를 혼용하지 않는다.

## 4. 실행

Visual Studio에서 시작 프로젝트를 `Client`로 지정하고, 디버깅 작업 디렉터리를 **`$(ProjectDir)..\Bin`**으로 설정한다.

```powershell
Set-Location .\Client\Bin
.\Client.exe
```

`Engine.dll`, `assimp-vc143-mtd.dll`, `fmodL.dll`과 모델·텍스처·바이너리 맵 데이터가 필요하다. 실행 파일만 다른 폴더로 옮기지 않는다. MapTool과 AnimTool도 각각 자신의 `Bin` 폴더를 작업 디렉터리로 사용한다.

## 5. 동작 확인

- 비동기 로딩 종료 후 게임 장면 진입
- 플레이어 이동, 셀 경계 이동과 슬라이딩
- 애니메이션 전환·전투·몬스터 및 보스
- 트레일, 파티클, 글로우·디졸브·물 디스토션
- MapTool의 데이터 저장·읽기 및 게임에서의 로딩

## 6. 문제 해결

| 증상 | 확인 항목 |
| --- | --- |
| v143 또는 SDK 오류 | Visual Studio 설치 구성 |
| FBX 헤더 없음 | FBX SDK 2020.3.7 설치 위치와 프로젝트 포함 경로 |
| LIB 파일 없음 | Git LFS, ThirdPartyLib, EngineSDK/Lib 및 엔진 빌드·복사 순서 |
| DLL 없음 | 실행 폴더의 Engine·Assimp·FMOD 런타임 파일 |
| 리소스 로딩 실패 | 실행 작업 디렉터리, LFS 복원, Bin 하위 리소스 |
| 도구에서 저장한 데이터가 다르게 표시 | 게임과 도구의 바이너리 데이터 구조 및 빌드 버전 일치 여부 |

이 절차는 프로젝트 설정에 근거한다. 새 PC의 clean build, Release 구성, 비동기 로딩과 전체 플레이는 별도 검증 대상이다. 외부 SDK와 게임 리소스는 각각의 이용 조건을 따른다.
