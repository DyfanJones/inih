test_that("write_ini write ini to file", {
  temp_file <- tempfile(fileext = '.ini')
  on.exit(unlink(temp_file))
  ini <- list(foo = list(bar = 123, cho = "hello world"))
  write_ini(ini, temp_file)

  expect_equal(
    readLines(temp_file), c("[foo]", "bar = 123", "cho = hello world", "")
  )
})

test_that("write_ini write empty ini to file", {
  temp_file <- tempfile(fileext = '.ini')
  on.exit(unlink(temp_file))
  write_ini(list(), temp_file)

  expect_equal(readLines(temp_file), character(0))
})
