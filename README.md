# GridCrawler

<img width="356" height="200" alt="img0" src="https://github.com/user-attachments/assets/af24212c-f36f-4bf6-9eb7-d8fbe147d380" />
<img width="356" height="200" alt="img1" src="https://github.com/user-attachments/assets/80151e60-6593-45d5-841c-abe7876d7fe4" />
<img width="356" height="200" alt="img2" src="https://github.com/user-attachments/assets/0e34dea1-6cb2-460f-94b0-4d75bc7dadf5" />
<img width="356" height="200" alt="img3" src="https://github.com/user-attachments/assets/13fec04b-afef-4be1-b4b5-fe92868bca95" />


**프로젝트 개요**
언리얼 엔진 5를 이용해 개발한 3D 탑다운 로그라이크 게임.

격자(Grid) 기반 이동과 스킬 시스템을 중심으로 구현했으며, 다수의 적 AI가 동시에 등장하는 전투를 고려해 경로 탐색 최적화와 확장 가능한 스킬 시스템을 설계했다.

- 시연 영상: https://youtu.be/D3uNCjeg6vk?si=uPEh_aadFPJ1cMt5


**담당 작업**
- 전투 및 스킬 시스템 구현
- AI 시스템 구현
- UI 구현
- 성능 최적화

# 기술적 사항

## 1. Strategy 패턴을 활용한 조합형 스킬 시스템 설계

### 문제

다양한 대상과 효과를 가진 스킬을 각각 구현하면 대상 지정과 효과 처리 로직이 하나의 클래스에 함께 포함된다.

이 경우 작동 방식이 비슷한 스킬을 제작할 때마다 유사한 코드가 반복되고, 이 부분을 변경할 경우 관련된 모든 스킬 클래스를 수정해야 하므로 유지보수 비용이 증가할 수 있다.
따라서 기존 시스템 수정 없이 새로운 대상 지정 방식과 효과를 추가할 수 있는 스킬 구조가 필요했다.

### 해결

<img width="673" height="232" alt="SkillData 구조" src="https://github.com/user-attachments/assets/c12e5962-066f-46b3-9237-c92e59ad308c" />

Strategy 패턴을 적용하여 스킬 대상(Delivery)과 스킬 효과(Effect)를 각각 독립적인 클래스로 분리하고 이를 조합해 하나의 스킬을 구성하도록 설계했다.
스킬 범위 계산과 대상 탐색은 Delivery가 담당하고 실제 효과 적용은 Effect가 담당하도록 역할을 나누어 각 기능들을 관리할 수 있도록 만들었다.

### 결과

<img width="1221" height="391" alt="SkillData 조합들" src="https://github.com/user-attachments/assets/2ec461fa-e6e1-4233-be51-c4b57cdd3dfb" />

프로젝트에서는 Delivery 3종과 Effect 3종을 조합해 총 16개의 스킬을 제작했다.

기존 Delivery와 Effect를 재사용하거나 필요한 파생 클래스만을 구현하면 새로운 스킬을 추가할 수 있도록 확장성을 확보했다.
Delivery 1종과 Effect 1종의 조합만으로도 9개의 스킬을 제작함으로써 기능 재사용성 또한 확인할 수 있었다.


## 2. Flow Field 기반 AI 경로 탐색 최적화

### 문제

모든 적이 개별적으로 A*를 수행하는 구조에서는 동일한 목적지에 대한 중복 계산이 반복되었고, 적이 늘어날수록 계산에 따른 CPU 소요 시간이 점점 증가했다.
이에 따라 다수의 적이 등장하는 상황에서 경로 탐색 비용을 줄일 수 있는 최적화가 필요했다.

### 해결

<img width="941" height="401" alt="FlowField 다이어그램" src="https://github.com/user-attachments/assets/9990c87a-2e59-4997-b083-2c7bcaca16d1" />

플레이어 위치를 기준으로 BFS를 수행해 Distance Field를 생성하고, 이를 기반으로 각 타일의 이동 방향을 기록한 Flow Field를 구성했다.

일반적인 이동은 Flow Field의 방향 정보만 참조해 움직인다. 다른 적으로 인해 일정 횟수 이상 이동하지 못하는 경우에만 A*를 수행해 Flow Field에 다시 합류할 수 있는 경로로 우회하도록 역할을 분리했다.

### 결과

<img width="983" height="358" alt="FlowField 결과 그래프 및 표" src="https://github.com/user-attachments/assets/24476371-3669-4d6c-9122-0c03bf131503" />

성능 검증을 위해 50x50 테스트 맵에서 최대 1000마리의 적이 등장하는 환경을 구성하고 Distance Field를 1초마다 갱신하는 동일한 조건에서 300프레임 동안 평균 CPU 소요 시간을 측정했다.
그 결과 적 수에 따라 기존 방식 대비 약 55-63%의 평균 시간 감소를 확인했다.

다수의 적이 동일한 이동 정보를 공유하도록 설계해 경로 계산 횟수를 줄였으며 이동이 불가능한 상황에서는 즉시 A* 수행하지 않고 일정 시간 대기하도록 하여 빈번한 재탐색도 방지했다.
이를 통해 다수의 적이 동시에 이동하는 상황에서도 경로 탐색 비용 증가를 효과적으로 억제할 수 있다.


## 3. BattleManager 기반 스킬 실행 파이프라인 설계

### 문제

스킬은 범위 표시 -> 애니메이션 재생 -> 효과 처리의 단계를 거쳐 실행된다. 그러나 범위 표시 시스템과 애니메이션의 AnimNotify 이벤트는
현재 실행 중인 스킬의 정보를 알 수 없어 각 스킬에 맞는 범위와 효과를 독립적으로 처리할 수 없었다. 또한 하나의 스킬이 여러 효과를 가지는 경우
현재 몇 번째 효과를 실행해야 하는지 판단할 수 있는 정보도 필요했다. 따라서 스킬 실행에 필요한 정보들을 Context로 구성하여 중앙에서 관리하고
각 시스템이 참조할 수 있는 구조가 필요했다.

### 해결

<img width="1360" height="504" alt="BattleManager 파이프라인" src="https://github.com/user-attachments/assets/ce346c9e-7b1b-48be-95d3-047c08234958" />

BattleManager가 실행 중인 스킬 Context를 중앙에서 관리하는 파이프라인을 설계했다. 스킬이 시작되면 Skill ID를 기반으로 Context를 생성하고
범위 정보, 현재 실행 단계, 효과 정보 등을 저장하도록 구성했다. 범위 표시 시스템과 애니메이션은 직접 스킬 정보를 관리하지 않고 BattleManager를
통해 Context를 조회하여 필요한 범위와 효과 정보를 참조하도록 구현했다.

### 결과

<img width="1119" height="500" alt="BattleManager 예시" src="https://github.com/user-attachments/assets/454dcc90-d447-45cd-9271-066860ce9d2c" />

범위 표시, 애니메이션, 효과 처리로 분리된 각 시스템이 동일한 스킬 Context를 참조하게 되어 각 시스템이 동일한 스킬 Context를 참조하게 되어
각 시스템이 스킬 정보를 직접 관리하지 않고도 자신의 역할만 수행할 수 있었다. Skill ID를 기반으로 각 스킬의 Context를 개별적으로 관리하여
여러 캐릭터가 동시에 스킬을 사용하더라도 혼선없이 처리할 수 있었다. 또한 애니메이션의 Notify를 추가하거나 발생 시점만 조정하는 것으로
다단 히트, 연속 효과 등 다양한 연출을 구성할 수 있었다.
