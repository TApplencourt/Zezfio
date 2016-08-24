import json

def _byteify(data, ignore_dicts=False):
    # if this is a unicode string, return its string representation
    if isinstance(data, unicode):
        return data.encode('utf-8')
    # if this is a list of values, return list of byteified values
    if isinstance(data, list):
        return [_byteify(item, ignore_dicts=True) for item in data]
    # if this is a dictionary, return dictionary of byteified keys and values
    # but only if we haven't already byteified it
    if isinstance(data, dict) and not ignore_dicts:
        return {
            _byteify(key,
                     ignore_dicts=True): _byteify(value,
                                                  ignore_dicts=True)
            for key, value in data.iteritems()
        }
    # if it's anything else, return it in its original form
    return data

def json_load_byteified(file_handle):
    return _byteify(
        json.load(file_handle,
                  object_hook=_byteify),
        ignore_dicts=True)

def get_json(path):
    with open(path) as data_file:
        data = json_load_byteified(data_file)
    return data
