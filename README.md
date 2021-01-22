# Factorio-Imitation

## 사용한 라이브러리
* WinAPI (Windows API)
* [GLM (OpenGL Mathematics)](glm.g-truc.net/0.9.8/index.html)
* [GLEW (OpenGL Extension Wrangler Library)](glew.sourceforge.net/)
* [stb_image (Image Loader)](https://github.com/nothings/stb#stb)

## 사용한 기술
* Design Pattern
    * 팩토리 메소드 (Factory Method)
    * 싱글톤 (Singleton)

* OpenGL  
    * 셰이더 (GLSL)
        * Vertex Buffer Object
        * Uniform Buffer Object
    * [인스턴싱 (Instancing)](#인스턴싱-Instancing)
    
* STL
   * vector
   * map
   * multimap
   
## 구현 목록
* [캐릭터 (Character)](#캐릭터-character)
* 구조물 (Structure)
    * 화력 채광 드릴 (Burner Mining Drill)
    * 운송 벨트 (Transport Belt)
    * 투입기 (Inserter)
    * 돌 용광로 (Stone Furnace)
    
    
## 정의부

### OpenGL(Open Graphics Library)
* 하드웨어 가속으로 컴퓨터 그래픽을 처리
* 2D, 3D 그래픽 라이브러리

### 인스턴싱 (Instancing)
* 서로 비슷한 요소들을 한번에 GPU로 넘겨주어 한번에 렌더한다.
* CPU와 GPU간의 통신이 반복되지 않아 딜레이가 적어 속도가 빠르다.

* 기술이 사용된 객체: 타일(Tile), 광물(Ore), 땅 위의 아이템(Item On Ground)

### 캐릭터 (Character)
* 조작
   * 이동
   * 건설
   * 철거
   
* 애니메이션
   * 유휴 (Idle)
   * 달리기 (Running)
   
* 인벤토리
   * 보유중인 아이템을 획득하면 기존의 아이템과 합쳐진다.

### 구조물 (Structure) / 화력 채광 드릴 (Burner Mining Drill)
* 타일에 있는 광물을 채취한다.
* 연료를 에너지로 사용한다.
* 배출구로 광물을 배출한다.
