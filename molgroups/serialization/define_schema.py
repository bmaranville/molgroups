"""
Support for serializing classes with dataclasses.
"""
import os

# this can be substituted with pydantic dataclass for schema-building...
USE_PYDANTIC = os.environ.get('USE_PYDANTIC', "False") == "True"
if USE_PYDANTIC:
    from pydantic.dataclasses import dataclass
else:
    from dataclasses import dataclass
from dataclasses import field, fields, is_dataclass, Field

try:
    from typing import Literal, Protocol, runtime_checkable
except ImportError:
    from typing_extensions import Literal, Protocol, runtime_checkable
from typing import Optional, Type, TypeVar, Any, Union, Dict, Callable, Tuple, List, Sequence

# decorator to tie model classes to implementations:
def implementation(cls):
    schema = cls.mro()[1]  # schema must be first
    # if it is inherited from another schema with no new
    # fields, there will be no __annotations__ dict yet:
    annotations = getattr(schema, '__annotations__', {})
    annotations['type'] = Literal[cls.__name__]
    schema.__annotations__ = annotations
    #schema.__annotations__['type'] = Literal[cls.__name__]
    cls.type = cls.__name__
    schema.__name__ = cls.__name__
    dataclass(eq=False, init=False)(schema)
    return cls

def field_desc(description: str) -> Any:
    return field(metadata={"description": description})

T = TypeVar('T')

def schema(
        *,
        include: Optional[List[str]] = None,
        exclude: Optional[List[str]] = None,
        classname: Optional[str] = None,
        eq: bool = True,
        init: bool = False
    ) -> Callable[[Type[T]], Type[T]]:
    
    """ 
    Create a dataclass from a subset of field names.
    fields should be a list of strings corresponding to attribute names,
    or if fields is None, all annotated attributes will be used as fields

    if attribute "schema_description" is found, will be used for description of
    generated schema, or else the class docstring will be used.
    """
    def set_dataclass(cls: Type[T]) -> Type[T]:
        realname = cls.__name__
        name = realname if classname is None else classname
        # all_annotations = dict([(f.name, f.type) for f in fields(cls)])
        all_annotations = getattr(cls, '__annotations__', {})
        if include is not None:
            if exclude is not None:
                raise ValueError("include array and exclude array are mutually exclusive - only define one")
            field_annotations = dict([(k, all_annotations[k]) for k in include])
        elif exclude is not None:
            field_annotations = dict([(k, v) for k, v in all_annotations.items() if not k in exclude])
        else:
            field_annotations = dict([(k, v) for k, v in all_annotations.items() if not k.startswith('_')])
        # we want this at the end, always, since it has a default value:
        field_annotations.pop('type', None)
        field_annotations['type'] = Literal[name]
        print(field_annotations)
        setattr(cls, '__annotations__', field_annotations)
        setattr(cls, 'type', field(repr=False, default=name))
        has_init = hasattr(cls, '__init__')
        do_init = init and not has_init
        # optional temporary name change, which affects generated model:
        if name != realname:
            cls.__name__ = name
        dataclass(init=init, eq=eq)(cls)
        # set the name back, to match python globals:
        cls.__name__ = realname
        # HACK! Pydantic doesn't copy __doc__ into model
        if hasattr(cls, '__pydantic_model__'):
            model = getattr(cls, '__pydantic_model__')
            docstring = getattr(cls, 'schema_description', cls.__doc__)
            setattr(model, '__doc__', docstring)
        setattr(cls, '__annotations__', all_annotations)
        if not init and not has_init:
            # if the 'type' attribute is not going to be set by the 
            # dataclass-provided __init__, we will set it ourselves
            setattr(cls, 'type', name)
        return cls

    return set_dataclass

def has_schema(cls):
    return is_dataclass(cls)