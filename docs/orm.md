# ORM documentation

## Model

### Model definition

The definition of Model objects follow conventional C++ class definition.

The class fields have conventional C++ types which will be transformed to the equivalent SQL types.
No std::* types are allowed, besides std::string. Ideally, each field should be a primitive type, with a direct SQL
translation.
A comprehensive list of allowed C++ types will be provided in the future.

### Field properties

Each field of the Model class can be annotated with additional properties regarding its behaviour in the database.
For example, a `std::string` field may be annotated with a `max_length` property, which will be translated to
a `VARCHAR(max_length)` SQL type.
Similarly, fields can be annotated as required, nullable, primary key, and so on.

The properties of fields are specified through the inner class `FieldProperties`, which is a nested class of the Model
class.
The fields of `FieldProperties` should have the same name as the fields of the Model class; multiple properties can be
specified for the same field.

Example:

```c++
struct User {
    std::string username;
    std::string password;
    std::string email;
    int age;
    
    struct FieldProperties {
        PropertiesList username = {Required(), MaxLength(50)};
        PropertiesList password = {Required(), MaxLength(50)};
        PropertiesList age = {Required()};
        PropertiesList unknown_field = {Required()};
    };
};
```

Here, email does not have any constraints, so it is not annotated with any properties. The field `unknown_field` is not
part of the Model class, so it will be ignored by the ORM (with a warning(?)).

### Model properties

Similarly to field properties, the Model class can be annotated with additional properties regarding its behaviour in
the database.
For example, a Model class can be annotated with a `table_name` property, which will be translated to the equivalent SQL
table name.
The properties of the Model class are specified through the inner class `ModelProperties`, which is a nested class of
the Model class.
Here, each property is a field of the inner type.
For example,

```c++
struct User {
    std::string username;
    std::string password;
    std::string email;
    int age;
    
    struct ModelProperties {
        static constexpr std::string_view table_name = "users";
        static constexpr auto primary_key = std::make_tuple(&User::username);
    };
};
```

A comprehensive list of allowed properties will be provided in the future.