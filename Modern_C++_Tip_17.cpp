// C의 특성을 물려받은 C++는 예외에 대한 책임은 온전히 프로그래머의 몫이다.(자유를 준 만큼 책임도)
// 하지만 그로 인해 생긴 try catch문은 유지보수의 문제로 엥간해선 사용되지 않는다.
// 코드가 수정되면 그에 따라 발생할 수 있는 예외도 수정되어야 하고 그렇게 되면 기존 프로그램과도 호환되지 않을 수 있기 때문이다.
// 하지만 시간이 흐르면서 프로그래머들은 예외가 방출되지 않는다는 조건만은 큰 도움이 된다는 것을 알게되었다.
// 클라이언트의 입장에서 사용하고자 하는 함수가 예외를 절대 발생시키지 않는 지, 하나라도 발생시키는 지의 여부는 굉장히 중요하다.
// 그리고 noexcept 키워드는 그런 배경을 바탕으로 탄생한 키워드로써, 이 함수가 예외를 발생시키지 않는다는 것을 모두에게 알려준다.
// noexcept 키워드는 const 키워드 만큼 중요하다. 

// 기존 C++98에서는 예외를 발생시키지 않는 함수에 대해서 빈 throw문으로 표시했다.
RetType function(params) throw();
// 그리고 C++11에서는 noexcept라는 키워드를 탄생시켰다.
RetType function(params) noexcept;

// noexcept 키워드는 최적화 측면에서 기존 방식보다 유리하다.
RetType function(params) noexcept;						// 최적화 잘 됨
RetType function(params) throw();						// 최적화 덜 됨
RetType function(params);								// 최적화 덜 됨
// noexcept 함수에서 컴파일러의 최적화기(optimizer)는 예외가 함수 바깥으로 전파될 수 있다고 해도 실행시점 스택을 풀기 가능 상태로 유지할 필요가 없다. 
// 또한, 예외가 noexcept 함수를 벗어난다고 해도 noexcept 함수 안의 객체들을 반드시 생성의 반대 순서로 파괴해야 하는 것도 아니다.

// 몇몇 함수에 대해서는 최적화는 더 두드러진다.
std::vector<Widget> vw;
…
Widget w;
…
vw.push_back(w);
// std::vector는 현재 용량이 꽉 찼을 때 element를 추가하면 동적할당으로 capacity를 더 큰 크기로 재할당하고, 기존 요소들을 복사하는 방식으로 동작한다.
// 하지만 복사하는 방식은 기존 크기 만큼의 임시 객체 생성하므로 비용이 적지 않게 들었다.

// 따라서 C++11에서는 자연스러운 컴파일러 최적화 방식으로 복사 대신 이동을 하는 것으로 std::vector의 동작 방식을 대체하였다.
// 하지만 이는 큰 문제가 될 수 있다.
// 만약 이동 도중 예외가 발생하여 기존 요소가 모두 옮겨지지 않았다면, 예외 안전성을 지켰다고 볼 수가 없다.
// 그래서 컴파일러는 정의된 이동 함수가 예외를 발생시키지 않는지를 확인을 한 뒤에 최적화 방식을 결정한다.
// 그리고 그 컴파일러가 최적화를 적용할 지를 결정할 함수의 예외를 발생시키는 지의 여부를 확인하는 방법이 바로 noexcept 키워드이다.
// 여러 표준 함수는 "가능하면 이동하고 아니면 복사한다" 전략을 활용한다.

// std::swap의 noexcept 여부는 사용자 정의 swap의 noexcep여부에 따라 결정된다.
template<class T, size_t N>								// Widget 배열들에 대한 swap
void swap(T (&a)[N],
		  T (&b)[N]) noexcept(noexcept(swap(*a, *b)));
// 이 함수가 noexcept인지 여부는 noexcept절 안의 표현식이 noexcept인 지에 의존한다.
// 따라서 Widget 클래스 내부에 정의된 Widget끼리 swap하는 함수가 noexcept인 지의 여부에 따라
// Widget 배열의 swap도 noexcept인 지가 결정된다.

// 대부분의 함수는 예외 중립적이다.
// 예외 중립적인 함수는 스스로 예외를 던지지는 않지만, 예외를 던지는 함수를 호출할 수는 있다.
// 다른 함수가 예외를 던지면 예외 중립적 함수는 그 예외를 그대로 통과시킨다.
// 이러한 함수는 절대 noexcept가 될 수 없다.

// 이런 함수를 억지로 예외를 던지지 않는 함수로 수정하려고 하면 구현이 매우 복잡해지고 유지보수가 힘들어질 수 있다.
