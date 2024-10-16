# Simple HTTP Server
⚠️ 외부로 부터 접속 시도가 오지 않는 로컬 환경 내에서 실행을 권장합니다.⚠️

## Usage
`./httpd [--port=n] [--chroot --user=u --group=g] [--debug] <docroot>`

## Functions

### main.c

`main.c`는 `getopt_long()`을 사용해 커맨드 라인의 옵션을 파싱한 후, 옵션에 따른 서버 환경설정을 수행하는 프로그램입니다.

- `--port` (DEFAULT : 80) : 요청을 listen할 포트를 설정
- `--chroot` : chroot를 활성화
- `--user` and `--group` : 자격 증명을 user와 group로 변경
- `--debug` (DEFAULT : 0) : 디버그 모드를 활성화

### server.c

#### setup_environment

`setup_environment()`는 `--chroot` 옵션에 의해 실행됩니다.

`chroot()`를 통해 프로세스가 접근할 수 있는 파일 시스템을 `docroot` 제한해 보안을 강화합니다.

이후 `setgid()` → `initgroups()` → `setuid()` 순으로 자격 증명을 지정한 `user`와 `group`으로 변경합니다.

#### listen_socket

`listen_socket()`은 `getaddrinfo()`를 통해 현재 호스트의 주소를 얻은 후, `socket()` → `bind()` → `listen()` 순으로 `--port`로 지정한 포트에서 요청을 대기하는 소켓을 생성합니다.

#### become_daemon

`become_daemon()`은 `setsid()`를 통해 제어 단말을 가지지 않는 새로운 세션의 리더가 되는 동시에 데몬 프로세스가 됩니다.

이때 현재 프로세스가 프로세스 그룹 리더라면 `setsid()`가 실패하기 때문에 `fork()`를 통해 실패를 방지합니다.

#### server_main

`server_main()`은 `accept()`를 통해 접속이 성립되면 새로운 소켓을 연 후, `fork()`로 자식 프로세스를 생성하고 자식 프로세스에서 `service()`를 수행합니다.

#### service

`service()`는 `read_request()`로 HTTP 요청을 읽고 해당 요청에 대해 `respond_to()`로 응답 합니다.

### request.c

#### read_request

`read_request()`는 HTTP 요청을 읽고 요청 라인(request line), 요청 헤더(request header), 요청 본문(request body)로 파싱합니다.

### response.c

#### response_to

`response_to()`는 HTTP 요청 메서드에 따라 응답을 처리합니다.