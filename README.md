# GridCrawler
<img width="356" height="200" alt="img0" src="https://github.com/user-attachments/assets/af24212c-f36f-4bf6-9eb7-d8fbe147d380" />
<img width="356" height="200" alt="img1" src="https://github.com/user-attachments/assets/80151e60-6593-45d5-841c-abe7876d7fe4" />
<img width="356" height="200" alt="img2" src="https://github.com/user-attachments/assets/0e34dea1-6cb2-460f-94b0-4d75bc7dadf5" />
<img width="356" height="200" alt="img3" src="https://github.com/user-attachments/assets/13fec04b-afef-4be1-b4b5-fe92868bca95" />

격자형 필드에서 플레이어와 다수의 적이 스킬을 사용해 전투하는 3D 탑다운 로그라이크 게임. 언리얼 엔진 5로 개발했으며 모듈화된 기능을 조합해 다양한 스킬을 효율적으로 제작하도록 구성하고, 스킬 실행 흐름을 중앙에서 관리하도록 설계함. 또한 적들이 경로 정보를 공유하는 추적 방식을 적용해 경로 탐색 비용을 줄임

## 참고 링크
- 플레이 영상: https://youtu.be/8sgF806Z--w

## 핵심 기술 구현

### 1. Flow Field 기반 AI 경로 탐색 최적화
<img width="627" height="267" alt="FlowField 다이어그램" src="https://github.com/user-attachments/assets/e357c7c1-25b2-4b14-8e1e-3bb8b1789690" />
<img width="655" height="239" alt="FlowField 결과 그래프 및 표" src="https://github.com/user-attachments/assets/19af3ab5-b217-49a3-9f69-3973622ddad7" />

#### [문제 상황]
- 기존 구조에서는 적이 플레이어를 추적하기 위해 타일을 한 칸 이동할 때마다 개별적으로 A* 경로 탐색을 수행함
- 적 수가 늘어날수록 반복적인 경로 계산으로 인해 CPU 소요 시간이 증가함
- 다수의 적이 등장하는 상황에서도 효율적으로 플레이어를 추적할 수 있도록 경로 탐색 비용을 줄일 필요가 있었음
#### [해결 과정]
- 플레이어 위치를 기준으로 BFS를 수행해 타일별 거리를 기록한 Distance Field를 생성하고, 거리가 가장 많이 줄어드는 인접 타일 방향을 Flow Field에 저장함
- 모든 적이 Flow Field를 공유해 이동 방향을 결정하되, 타일당 한 캐릭터만 위치할 수 있으므로 다른 적에게 막히면 일정 횟수 동안 대기하도록 구현함
- 대기 후에도 이동이 불가능하면 A*를 수행해 플레이어까지의 거리가 현재 타일과 같거나 더 짧은 주변 타일로 우회함
#### [결과]
- 50x50 테스트 맵에 다수의 적이 등장하는 4가지 환경을 구성하고, Flow Field를 1초마다 갱신하며 300프레임 동안 평균 CPU 소요 시간을 측정함
- 모든 적이 이동할 때마다 A*를 수행하던 기존 방식 대비 약 55~63%의 시간 감소를 확인함
- 다수의 적이 경로 정보를 공유해 탐색 비용을 낮추고, 막힘 발생 시 대기 후 우회하도록 설계해 불필요한 A* 수행을 줄임
<br></br>
### 2. 조합형 스킬 시스템 설계
<img width="449" height="154" alt="SkillData 구조" src="https://github.com/user-attachments/assets/1b278f07-06a9-439c-97bd-04a8537365b9" />
<br></br>
<img width="814" height="261" alt="SkillData 조합들" src="https://github.com/user-attachments/assets/55bb4c9f-2600-43b4-be84-3d64d9dd13a7" />

#### [문제 상황]
- 스킬을 개별 클래스로 구현하면 작동 방식이 비슷한 스킬마다 코드가 중복됨
- 코드 중복으로 인해 스킬 로직 변경 시 유지보수 비용이 증가함
- 핵심 구조를 수정하지 않고도 새로운 효과를 가진 스킬을 추가할 수 있는 시스템이 필요함
#### [해결 과정]
- 스킬 대상 선정과 효과 적용을 각각 Delivery와 Effect 추상 클래스로 정의함
- Delivery는 스킬 범위 계산과 대상 탐색을, Effect는 실제 효과 적용을 담당하도록 역할을 분리함
- 각 추상 클래스의 파생 구현체를 조합해 하나의 스킬을 만들 수 있도록 설계함
#### [결과]
- Delivery 3종과 Effect 3종을 조합해 총 16개의 스킬을 제작함
```
예) [Delivery: 범위] + [Effect: 피해] + [Effect: 이동] => 범위 내 대상에게 피해를 주면서 밀치는 스킬
예) [Delivery: 자신] + [Effect: 상태효과] => 사용자에게 버프를 부여하는 스킬
```
- 동일한 Delivery 1종과 Effect 1종의 조합만으로도 9개의 스킬을 제작해 기능 재사용성을 확인함
- 새로운 기능이 필요할 때, 파생 클래스를 추가하는 방식으로 확장성을 확보함
<br></br>
### 3. 스킬 실행 파이프라인 설계
<img width="906" height="336" alt="BattleManager 파이프라인" src="https://github.com/user-attachments/assets/309a887f-5365-4b82-a65c-1a9d6cfe761d" />

#### [문제 상황]
- 스킬은 범위 표시 -> 애니메이션 재생 -> 효과 처리 순서로 실행되지만, 각 처리를 담당하는 매니저와 애니메이터가 분리되어 실행 단계를 공유하기 어려움
- 스킬 범위 표시/제거 시점과 처리할 효과의 순서를 판단하려면 각 클래스에서 스킬 실행 정보를 확인할 수 있어야 함
- 스킬 실행 흐름에 맞춰 각 시스템에 관련 정보를 전달할 수 있는 수단이 필요했음
#### [해결 과정]
- 스킬 실행 정보를 담은 Context를 중앙에서 관리하고 각 처리 단계를 연결하는 BattleManager를 구현함
- 스킬 사용 시 고유한 SkillID를 가진 Context를 생성하고, 스킬 범위 정보와 사용자/대상 참조 등을 저장함
- 캐릭터의 스킬 애니메이션 이벤트가 발생하면 관련 클래스에 Context를 전달해 현재 단계에 필요한 작업을 요청함
#### [결과]
- BattleManager를 통해 실행 흐름을 조율하고 단계별로 필요한 데이터를 시스템에 전달함
- 각 클래스는 전달받은 Context와 요청을 바탕으로 자신이 담당하는 역할을 독립적으로 수행함
- SkillID로 개별 스킬 실행을 구분해 다수의 스킬도 혼선 없이 관리할 수 있음
