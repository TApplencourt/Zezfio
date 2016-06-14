import irpy
from zezfio.database import LegacyFolderHieracy
from zezfio.convert import data_str2py, data_py2c, data_mv2c, data_c2py
from zezfio.convert import dimension2len
from ctypes import c_int

d_instance = dict()

db= LegacyFolderHieracy("{{db_path}}")
{% for category, attributes in json_config.iteritems() %}

class {{ category|capitalize }}(object):

    {% for variable in attributes["attributes"] %}

    @irpy.lazy_property_mutable
    def {{ variable.name }}(self):
    {% if variable.default is not defined %}
        data_str = db.read({{ category }}, '{{ category }}', '{{ variable.name }}')
        return data_str2py(data_str, '{{ variable.type }}')
    {% else %}
        try:
            data_str = db.read({{ category }}, '{{ category }}', '{{ variable.name }}')
        except IOError:
            return {{ variable.default }}
        else:
            return data_str2py(data_str, '{{ variable.type }}')
    {% endif %}

    @irpy.lazy_property
    def {{ variable.name }}_c(self):
        return data_py2c(self.{{ variable.name }}, '{{ variable.type }}')

    @irpy.lazy_property
    def {{ variable.name }}_csze(self):
        ar = self.{{ variable.name }}_c
        return c_int(ar.buffer_info()[1] * ar.itemsize)

    @irpy.lazy_property
    def {{ variable.name }}_dimension(self):
        return {{variable.dimension}}

    def set_{{ variable.name }}(self,data_raw):
        ar = data_mv2c(data_raw, '{{ variable.type }}')

        ref_len = dimension2len(self.{{ variable.name }}_dimension)
        new_len = len(ar)

        if new_len == ref_len:
            data_py = data_c2py(ar, self.{{ variable.name }}_dimension)
            self.{{ variable.name }} = data_py
            db.write(self,'{{ category }}', '{{ variable.name }}', data_py)
        else:
            raise IndexError("{{ variable.name }} have not the right number of variable (old:%s vs new:%s)" % (ref_len,new_len) )

    {% endfor %}

{{ category }} = {{ category | capitalize }}()
d_instance[ "{{ category }}" ] = {{ category }}

{%  endfor %}
