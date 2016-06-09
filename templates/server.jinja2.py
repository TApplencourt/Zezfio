import irpy
from src.database import LegacyFolderHieracy
from src.convert import data_str2py, data_py2c, data_mv2c, data_c2py
from src.convert import dimension2len,aors2len

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
    def {{ variable.name }}_ctype(self):
        return data_py2c(self.{{ variable.name }}, '{{ variable.type }}')

    @irpy.lazy_property
    def {{ variable.name }}_dimension(self):
        return {{variable.dimension}}

    def set_{{ variable.name }}(self,data_raw):
        c = data_mv2c(data_raw, '{{ variable.type }}',self.{{ variable.name }}_dimension)
        new_len = aors2len(c)
        ref_len = dimension2len(self.{{ variable.name }}_dimension)

        if new_len == ref_len:
            self.{{ variable.name }} = data_c2py(c)
            db.write(self,'{{ category }}', '{{ variable.name }}', c)
        else:
            raise IndexError("{{ variable.name }} have not the right number of variable (%s vs %s)" % (ref_len,new_len) )

    {% endfor %}

{{ category }} = {{ category|capitalize }}()
d_instance[ "{{ category }}" ] = {{ category }}

{%  endfor %}

