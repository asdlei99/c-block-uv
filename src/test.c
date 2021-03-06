/* Copyright (c) 2014, huxingyi@msn.com
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "c-block-uv.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct task {
  struct c_block block;
  union {
    c_fs_stat_t stat;
  } u;
};

int task_clousre(struct task *ctx) {

  printf("enter task_clousre\n");
  
  c_begin(ctx, task_clousre);
  
  printf("before c_fs_stat\n");
  
  ctx->u.stat.loop = uv_default_loop();
  ctx->u.stat.path = "./test.exe";
  c_await(&ctx->u.stat, c_fs_stat);
  
  printf("stat result:%d\n", ctx->u.stat.req.result);
  
  uv_fs_req_cleanup(&ctx->u.stat.req);
  
  printf("after c_fs_stat\n");
  
  c_end();
  
  return c_finished(ctx);
}

static int on_final(struct task *ctx) {
  printf("free ctx\n");
  free(ctx);
  return 0;
}

void test_c_fs_stat(void) {
  struct task *ctx = (struct task *)malloc(sizeof(struct task));
  c_spawn(ctx, task_clousre, on_final);
}

int main(int argc, char *argv[]) {

  printf("enter\n");
  
  test_c_fs_stat();

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  
  printf("leave\n");
  
  return 0;
}
