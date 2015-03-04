#include "Timer.hpp"

#include <uv.h>
#include <stdio.h>

using namespace std;

const int N = 10;
const int WIDTH = 1024, HEIGHT = 1024;

uv_fs_t open_req;
uv_fs_t read_req;

struct data_info
{
    int count;
    uv_buf_t *buffer;
};

uv_work_t process_req;

static float buffer[WIDTH * HEIGHT];
static uv_buf_t iov;

void on_open(uv_fs_t *req);
void on_read(uv_fs_t *req); 
void process(uv_work_t *req);
void after_process(uv_work_t *req, int status);

void on_open(uv_fs_t *req) 
{
    if (req->result >= 0) 
    {
        iov = uv_buf_init((char *)buffer, sizeof(buffer));

        data_info *info = new data_info;
        info->count = 0;
        info->buffer = &iov;
        read_req.data = info;

        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    }
    else 
    {
        fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
    }
}

void on_read(uv_fs_t *req) 
{
    if (req->result < 0) 
    {
        fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
    }
    else if (req->result == 0) 
    {
        uv_fs_t close_req;

        // synchronous
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    }
    else if (req->result > 0) 
    {
        data_info *info = (data_info *)req->data;
        printf("on_read() count:%d read: %ld\n", info->count, req->result);

        if (info->count < N)
        {
            // process
            process_req.data = info;
            uv_queue_work(uv_default_loop(), &process_req, process, after_process);

            // read next block
            info->count++;
            uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
        }
        else
        {
            uv_fs_t close_req;

            // synchronous
            uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
        }
    }
}

void process(uv_work_t *req) 
{
    data_info *info = (data_info *)req->data;

    printf("process() count:%d\n", info->count);
}

void after_process(uv_work_t *req, int status) 
{
    puts("after_process()");
}

int main() 
{
    puts("Hello libuv!");

    // open file
    uv_fs_open(uv_default_loop(), &open_req, "network/input.dat", O_RDONLY, 0, on_open);

    Timer t0("all");

    // event loop
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    t0.print();

    uv_fs_req_cleanup(&open_req);
    uv_fs_req_cleanup(&read_req);

    return 0;
}