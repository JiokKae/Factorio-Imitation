# Factorio-Imitation

## 라이브러리
* [WinAPI (Windows API)](https://docs.microsoft.com/en-us/windows/win32/)
* [OpenGL](https://www.opengl.org/)
* [GLM (OpenGL Mathematics)](https://github.com/g-truc/glm)
* [GLEW (OpenGL Extension Wrangler Library)](http://glew.sourceforge.net/)
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
#### [캐릭터 (Character)](#캐릭터-character-1)
#### 구조물 (Structure)
* [화력 채광 드릴 (Burner Mining Drill)](#화력-채광-드릴-burner-mining-drill)
* [운송 벨트 (Transport Belt)](#운송-벨트-transport-belt)
* [투입기 (Inserter)](#투입기-inserter)
* [돌 용광로 (Stone Furnace)](#돌-용광로-stone-furnace)
    
    
## 정의부

### OpenGL(Open Graphics Library)
* 하드웨어 가속으로 컴퓨터 그래픽을 처리
* 2D, 3D 그래픽 라이브러리

### 인스턴싱 (Instancing)
* 서로 비슷한 요소들을 한번에 GPU로 넘겨주어 한번에 렌더한다.  
* CPU와 GPU간의 통신이 반복되지 않아 딜레이가 적어 속도가 빠르다.  

* 기술이 사용된 객체: [타일(Tile)](#타일-tile), 광물(Ore), [땅 위의 아이템(Item On Ground)](#땅-위의-아이템-item-on-ground)

### [캐릭터 (Character)](#캐릭터-character)
* 조작
	* 이동
	* 건설
	* 철거
    
* 애니메이션
	* 유휴 (Idle)
	* 달리기 (Running)
   
* 인벤토리
	* 보유중인 아이템을 획득하면 기존의 아이템과 합쳐진다.

### [화력 채광 드릴 (Burner Mining Drill)](#구조물-structure)
	구조물 (Structure)/ 화력 채광 드릴 (Burner Mining Drill)
* 타일에 있는 광물을 채취한다.
* 연료를 에너지로 사용한다.
* 배출구로 광물을 배출한다.

### [운송 벨트 (Transport Belt)](#구조물-structure)
	구조물 (Structure)/ 운송 벨트 (Transport Belt)
* 벨트위에 올라가있는 아이템들을 진행 방향으로 이동시킨다.
* 벨트의 종류에 따라 속도가 다르다.
* 사방의 운송벨트와 상호 작용하여 진행 방향이 바뀐다

### [투입기 (Inserter)](#구조물-structure)
	구조물 (Structure)/ 투입기 (Inserter)
* 구조물에 아이템을 투입한다.
* 구조물에서 아이템을 반출한다.
* 방향에 따라 이미지가 회전한다.

### [돌 용광로 (Stone Furnace)](#구조물-structure)
	구조물 (Structure)/ 투입기 (Inserter)
* 굽기 제작법이 있는 아이템들을 가공한다.
* 연료를 에너지로 사용한다.
* 다른 구조물로부터 아이템을 받을 수 있다.

### [구조물 빌더 (Structure Builder)](#구조물-structure)
* 임시 구조물
	* 마우스 좌표에 임시로 구조물을 배치하여 표시한다.
* 활성화
	* 설치 가능한 아이템을 핸드에 들었을 때 활성화한다.
	
### 타일 매니저 (Tile Manager)
#### 청크 (Chunk)
* 타일 32 * 32개를 구성하는 단위
* 인스턴싱 기법으로 청크단위로 화면에 그려진다.
* 음수 좌표를 사용하기 위해 map을 사용해 저장한다.

#### 타일 (Tile)
* 그리드의 기본 단위
* 광물을 가지고 있다.
* 타일 위에 지어진 구조물의 링크를 저장한다.
#### 광물 (Ore)
* 광물의 종류와 양을 저장한다.

### 엔티티 매니저 (Entity Manager)
* 엔티티 저장
	* multimap 사용
	* 엔티티들이 왼쪽 위부터 출력되어야한다.
	
#### 땅 위의 아이템 (Item On Ground)
* 인스턴싱 기법을 사용하여 아이템들을 일괄적으로 렌더한다.

* 구조물 빌더
