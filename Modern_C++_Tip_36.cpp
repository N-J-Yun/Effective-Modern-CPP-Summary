// vector같은 컨테이너들은 데이터를 힙영역에 저장하고 그 데이터를 가리키는 포인터를 들고 있다.
// 따라서 이런 컨테이너들은 복사하는 것보다 이동하는 것이 훨씬 빠르다.
// 해당 포인터의 연결만 끊고 새로운 포인터에 연결시키면 되기 때문

// 하지만 std::array같은 경우는 내장 배열에 STL 인터페이스를 씌운 것인데
// 다른 표준 컨테이너들과 저장 방식이 근본적으로 다르다.
// std::array는 포인터가 아니라 데이터 전체를 해당 객체에 담고 있기 때문에 상수시간이 소요되는 다른 컨테이너와 다르게
// 선형 시간이 소요된다.(not constant but linear)
// 이동 연산이나 복사 연산이나 선형 시간이 소요되는 것은 변함이 없다는 의미이다.
// 이렇게 되면 이동 연산이 복사 연산보다 더 cheap하다는 것은 큰 의미가 없어지게 된다.

// std::string같은 경우는 상수 시간의 이동 연산과 선형 시간의 복사 연산을 제공한다.
// 이런 경우라면 복사 연산보다 이동 연산이 훨씬 cheap하다고 주장할 수 있겠지만 실상은 그렇지 않다.
// 문자열 구현 중에는 작은 문자열 최적화를 사용하는 것들이 많다.(SSO, small string optimization)
// SSO는 작은 문자열을 std::string객체 안의 버퍼 안에 저장하고 힙에 할당한 저장소는 사용하지 않는다.(동적 할당을 막을 수 있다.)
// 이런 구현을 사용하는 std::string객체는 작은 문자열의 경우 이동이 복사보다 빠르지 않다.

// 이처럼 이동 연산은 항상 효율적이지는 않다.
// 대표적으로 3가지 상황에서 그렇다.
// 1. 이동 연산이 없다: 이동할 객체가 이동 연산들을 제공하지 않는다 -> 이 경우 이동 요청은 복사 요청으로 대체된다.
// 2. 이동이 더 빠르지 않다: 이동할 객체의 이동 연산이 해당 복사 연산보다 빠르지 않다.
// 3. 이동을 사용할 수 없다: 이동이 일어나려면 이동 연산이 예외를 방출하지 않아야 하는 상황에서 해당 연산에 noexcept로 선언되어 있지 않다.

// 일반적 코드에서는(템플릿을 작성하는 경우같은) 대체로 이동 연산들이 존재하지 않고 저렴하지 않고 적용되지 않는다는 가정이 사실이다.
// 하지만 이동 연산의 지원상황을 미리 알 수 있는 경우에는 위 같은 가정을 고려하지 않아도 된다.