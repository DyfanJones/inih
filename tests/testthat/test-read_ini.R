test_that("read_ini parse ini files", {
  actual <- read_ini("data.ini")
  expected <- list(
    job = list(company = "ExampleCorp", title = "Engineer"),
    person = list(age = "30", city = "New York", name = "John Doe", userId = "123456")
  )
  expect_equal(actual, expected)
})

test_that("read_ini parse empty ini files", {
  expect_equal(read_ini("empty.ini"), list())
})

test_that("read_ini missing file", {
  expect_error(
    read_ini("made_up"),
    "Can't load the INI file"
  )
})
