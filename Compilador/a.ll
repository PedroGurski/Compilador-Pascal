declare i32 @printf(ptr noundef, ...)
declare i32 @__isoc99_scanf(ptr noundef, ...)
@read_int = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@write_int = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@read_float = private unnamed_addr constant [3 x i8] c"%f\00", align 1
@write_float = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

@r = global i32 0
@x = global i32 0
@y = global i32 0

define void @maior(i32 %_arg_a, i32 %_arg_b, ptr %_arg_c) {
  %_a = alloca i32
  store i32 %_arg_a, ptr %_a
  %_b = alloca i32
  store i32 %_arg_b, ptr %_b
  %_c = alloca ptr
  store ptr %_arg_c, ptr %_c
  %1 = load i32, ptr %_a
  %2 = load i32, ptr %_b
  %3 = icmp sgt i32 %1, %2
  %4 = zext i1 %3 to i32
  %5 = trunc i32 %4 to i1
  br i1 %5, label %L1, label %L2

L1:
  %6 = load i32, ptr %_a
  %7 = load ptr, ptr %_c
  store i32 %6, ptr %7
  br label %L3

L2:
  %8 = load i32, ptr %_b
  %9 = load ptr, ptr %_c
  store i32 %8, ptr %9
  br label %L3

L3:
  ret void
}

define i32 @main() {
  %1 = call i32 (ptr, ...) @__isoc99_scanf(ptr @read_int, ptr @x)
  %2 = call i32 (ptr, ...) @__isoc99_scanf(ptr @read_int, ptr @y)
  %3 = load i32, ptr @x
  %4 = load i32, ptr @y
  call void @maior(i32 %3, i32 %4, ptr @r)
  %5 = load i32, ptr @r
  %6 = call i32 (ptr, ...) @printf(ptr @write_int, i32 %5)
  ret i32 0
}
