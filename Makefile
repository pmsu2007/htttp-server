# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -g  # 디버깅 심볼을 포함하도록 -g 옵션 추가

# 소스 파일 리스트
SRCS = main.c server.c request.c response.c log_utils.c signal_utils.c common_utils.c

# 오브젝트 파일 리스트 (소스 파일명에서 .o 파일 생성)
OBJS = $(SRCS:.c=.o)

# 실행 파일 이름
TARGET = httpd

# 기본 빌드 목표
all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 청소 목표: 생성된 오브젝트 파일과 실행 파일을 삭제
clean:
	rm -f $(OBJS) $(TARGET)

# 의존성 처리
.PHONY: all clean
