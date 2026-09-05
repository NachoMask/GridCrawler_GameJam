# GridCrawler

<img width="356" height="200" alt="img0" src="https://github.com/user-attachments/assets/af24212c-f36f-4bf6-9eb7-d8fbe147d380" />
<img width="356" height="200" alt="img1" src="https://github.com/user-attachments/assets/80151e60-6593-45d5-841c-abe7876d7fe4" />
<img width="356" height="200" alt="img2" src="https://github.com/user-attachments/assets/0e34dea1-6cb2-460f-94b0-4d75bc7dadf5" />
<img width="356" height="200" alt="img3" src="https://github.com/user-attachments/assets/13fec04b-afef-4be1-b4b5-fe92868bca95" />

<br>

**프로젝트 개요**
<br>
언리얼 엔진 5를 이용해 개발한 3D 탑다운 로그라이크 게임.

격자(Grid) 기반 이동과 스킬 시스템을 중심으로 구현했으며, 다수의 적 AI가 동시에 등장하는 전투를 고려해 경로 탐색 최적화와 확장 가능한 스킬 시스템을 설계했다.

- 시연 영상: https://youtu.be/D3uNCjeg6vk?si=uPEh_aadFPJ1cMt5

<br>

**담당 작업**
- 전투 및 스킬 시스템 구현
- AI 시스템 구현
- UI 구현
- 성능 최적화

# 기술적 문제 해결
<br>

## 1. 조합형 스킬 시스템 설계

### 문제

각각의 스킬을 클래스 단위로 따로 구현하면 작동 방식이 비슷한 스킬마다 비슷한 코드가 반복적으로 만들어진다. 이 경우 스킬 로직 변경을 필요한 유지보수 비용이 증가하게 된다. 따라서 핵심 구조를 수정할 필요 없이 새로운 효과를 가진 스킬을 추가할 수 있는 스킬 시스템이 필요했다.

### 해결

<img width="673" height="231" alt="SkillData 구조" src="https://github.com/user-attachments/assets/b59be7b0-b178-4c7a-98be-9b65a8937b6e" />


스킬 대상(Delivery)과 스킬 효과(Effect)라는 각각의 독립적인 클래스로 만들고 이를 조합해 하나의 스킬을 만들 수 있게 설계했다. 스킬 범위 계산과 대상 탐색은 Delivery가, 실제 효과 적용은 Effect가 담당하도록 역할을 나누어 각 기능들을 개별적으로 관리할 수 있도록 만들었다.

### 결과

<img width="1221" height="391" alt="SkillData 조합들" src="https://github.com/user-attachments/assets/afbb9fbe-e9ef-4310-9ab3-3dee25d85858" />


프로젝트에서 Delivery 3종과 Effect 3종을 만들었고 이들을 조합해 총 16개의 스킬을 제작했다. 예를 들어, 특정 범위에 피해를 주는 스킬은 [Delivery: 범위] + [Effect: 피해]를 조합해서 만들 수 있고, 자신에게 상태효과를 부여하는 스킬은 [Delivery: 자신] + [Effect: 상태효과]를 조합해서 만들 수 있다.

Delivery 1종과 Effect 1종의 조합만으로도 9개의 스킬을 제작하여 기능 재사용성을 확인할 수 있었으며 새로 만들 스킬에 필요한 Delivery/Effect이 없다면 원하는 기능을 가진 파생 클래스를 구현해 조합할 수 있어 확장성도 확보되었다.


<br>

## 2. Flow Field 기반 AI 경로 탐색 최적화

### 문제

등장하는 적들이 플레이어를 추적하기 위해 개별적으로 A*를 수행하는 기존 구조를 개선하고 싶었다. 격자 타일을 한 칸 이동할 때마다 경로 계산을 반복함에 따라 적이 늘어날수록 계산에 소요되는 시간이 점점 증가했다. 다수의 적이 등장하는 상황을 고려해 경로 탐색 비용을 줄일 수 있는 최적화를 모색했다.

### 해결

<img width="941" height="401" alt="FlowField 다이어그램" src="https://github.com/user-attachments/assets/ba85befd-232d-44e5-bc9e-b4d006e1fbe6" />


플레이어 위치를 기준으로 BFS를 수행해 각 타일마다 플레이어와의 거리를 기록한 Distance Field를 생성했다. 이를 활용해 각 타일에서 거리가 줄어드는 인접 타일의 방향을 기록한 Flow Field를 만들었다. 이 두 Field는 플레이어의 위치(타일)가 바뀔 때마다 새로 계산해야 한다.

FlowField는 플레이어와의 거리가 줄어드는 방향이 기록되어 있으므로 적은 이를 참고해 이동 방향을 정한다. 하지만 모든 적들이 방향 정보대로 움직이면 결국 이동 경로 상에 다른 적이 위치해 움직일 수 없게 된다. (하나의 타일에는 하나의 캐릭터만 위치할 수 있기 때문)

FlowField를 따라 이동할 수 없는 상황이라면 우선 일정 시간(횟수)동안 대기하면서 막힌 상황이 해결되기를 기다린다. 이후 여전히 막힌 상황에 놓여 있는 경우, A*를 실행해 근처 타일 중에서 DistanceField의 거리가 동일하거나 더 가까운 타일을 향해 우회 이동을 실시한다.


### 결과

<img width="983" height="358" alt="FlowField 결과 그래프 및 표" src="https://github.com/user-attachments/assets/f7bfd14b-74b6-4f2a-8810-7afb94281568" />


성능 검증을 위해 50X50 테스트 맵에서 100-300-500-1000마리의 적이 등장하는 4가지 환경을 구성했다. 플레이어의 이동을 변수에서 배제하기 위해 FlowField는 1초마다 자체적으로 갱신하는 조건으로 300프레임 동안 평균 CPU 소요 시간을 측정했다. 그 결과 모든 적이 이동 할 때마다 A* 를 사용했던 기존 방식 대비 약 55%-63%의 평균 시간 감소를 확인했다.

플레이어 위치가 바뀔 때 한번 생성하는 정보를 다수의 적이 공유함으로써 경로 탐색 비용을 효과적으로 줄일 수 있었다. 적의 이동이 불가능한 상황에는 즉시 우회를 수행하기보다는 막힘 상태가 해소되기를 기다리는 딜레이를 추가하여 빈번하게 A*로 전환될 수 있는 부분도 억제했다.


<br>

## 3. BattleManager 기반 스킬 실행 파이프라인 설계

### 문제

스킬은 범위 표시 -> 애니메이션 재생 -> 효과 처리의 단계를 거쳐 실행된다. 하지만 각각의 처리를 담당하는 매니저 및 애니메이터는 각자 분리되어 있기 때문에 현재 어느 단계가 진행 중인지 알 수 없다.

스킬 범위를 언제 표시하고 언제 표시를 제거할지, 사용한 스킬의 몇 번째 효과를 처리해야 하는지를 각 클래스에서 알기 위해서는 스킬 실행과 관련된 Context 정보가 필요하다고 느꼈고 이 Context를 중앙에서 관리하고 각 시스템에 전달할 수 있는 설계를 만들고자 했다.

### 해결

<img width="1359" height="504" alt="BattleManager 파이프라인" src="https://github.com/user-attachments/assets/d306d1fb-fe4a-417c-9422-c8483a8a74fe" />


실행한 스킬의 Context를 중앙에서 관리하는 BattleManager라는 파이프라인을 만들었다. 

스킬을 사용하면 고유한 SkillID를 가진 Context가 만들어진다. Context에는 범위 표시에 필요한 스킬 범위 정보나 스킬 사용자 및 효과 대상에 대한 참조 등이 들어있다. Character의 스킬 애니메이션 이벤트가 발생하는 순간, 각각의 클래스들에게 Context를 전달해 효과 처리에 필요한 작업을 요청한다.

### 결과

<img width="1119" height="500" alt="BattleManager 예시" src="https://github.com/user-attachments/assets/186d9493-b0a7-4384-83a6-4c57cfea08ab" />

BatleManager가 스킬 사용 흐름에 맞춰 필요한 데이터가 담긴 Context를 각 클래스에 전달하기 때문에 클래스에서는 자신이 맡은 역할만 독립적으로 수행할 수 있다. SkillID를 통해 각 스킬을 구분해 관리하므로 여러 캐릭터가 동시 다수의 스킬을 사용하더라도 각각을 혼선없이 처리할 수 있었다. 또한 애니메이션의 이벤트를 추가하거나 발생 시점만 조정하는 것으로 다단 히트, 연속 효과 등 다양한 연출을 구성할 수 있었다.

<br>
