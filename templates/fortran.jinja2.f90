MODULE zezfio

  include 'f77_zmq.h'
  integer(ZMQ_PTR) :: context
  integer(ZMQ_PTR) :: responder

END module

subroutine ezfio_set_file(x)
  implicit none
  character*(*) :: x
  call ezfio_init()
end

subroutine ezfio_init()
  use zezfio

  implicit none

  character*(64) ::  address
  integer        ::  rc

  CALL getenv("EZFIO_ADDRESS", address)
  
  context   = f77_zmq_ctx_new()
  responder = f77_zmq_socket(context, ZMQ_REQ)
  rc        = f77_zmq_connect(responder,address)

  IF ( rc == -1 ) THEN
    print*, "Cannot connect to the server"
    STOP 1
  ENDIF

end subroutine ezfio_init


subroutine ezfio_finish()
  call ezfio_finalize()
end subroutine ezfio_finish

subroutine ezfio_finalize()
    use zezfio

    implicit none

    integer  ::  rc

    rc = f77_zmq_close(responder)
    rc = f77_zmq_ctx_destroy(context)

end subroutine ezfio_finalize

{% for category, attributes in json_config.iteritems() %}
  {% for variable in attributes["attributes"] %}
subroutine ezfio_has_{{category}}_{{variable.name}}(bool)

    use zezfio

    implicit none
    
    logical, intent(inout)  ::  bool

    integer ::  exit_code
    integer ::  rc

    rc = f77_zmq_send(responder, "has", 3, ZMQ_SNDMORE)
    if (rc /= 3) then
      print *, 'ZEZFIO error: ezfio_has_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "has", 3, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)
    if (rc /= {{ category | length }}) then
      print *, 'ZEZFIO error: ezfio_has_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)
    if (rc /= {{ variable.name | length }}) then
      print *, 'ZEZFIO error: ezfio_has_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)'
      stop 126
    endif

    rc = f77_zmq_recv(responder, exit_code, 4, 0)
    if (rc /= 4) then
      print *, 'ZEZFIO error: ezfio_has_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, exit_code, 4, 0)'
      stop 126
    endif

    if (exit_code < 0) then
      print *, 'ZEZFIO error: ezfio_has_{{category}}_{{variable.name}}'
      print *, '  exit_code = ', exit_code
      stop 126
    endif
    bool = (exit_code == 0)

end subroutine ezfio_has_{{category}}_{{variable.name}}

subroutine ezfio_size_{{category}}_{{variable.name}}(buffer)
  use zezfio

    implicit none
    
    {{ typec2stuff[variable.type].fortran }}, intent(inout)  ::  buffer(*)
    integer ::  rc
    integer ::  sze

    rc = f77_zmq_send(responder, "size", 4, ZMQ_SNDMORE)
    if (rc /= 4) then
      print *, 'ZEZFIO error: ezfio_size_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "size", 4, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)
    if (rc /= {{ category | length }}) then
      print *, 'ZEZFIO error: ezfio_size_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)
    if (rc /= {{ variable.name | length }}) then
      print *, 'ZEZFIO error: ezfio_size_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)'
      stop 126
    endif

    rc = f77_zmq_recv(responder, sze, 4, 0)
    if (sze < 0) then
      print *, 'ZEZFIO error: ezfio_size_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, sze, 4, 0) : sze = ', sze
      stop 126
    endif

end subroutine ezfio_size_{{category}}_{{variable.name}}

subroutine ezfio_get_{{category}}_{{variable.name}}(buffer)
  use zezfio

    implicit none
    
    {{ typec2stuff[variable.type].fortran }}, intent(inout)  ::  buffer(*)
    integer ::  rc
    integer ::  sze

    rc = f77_zmq_send(responder, "get", 3, ZMQ_SNDMORE)
    if (rc /= 3) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "get", 3, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)
    if (rc /= {{ category | length }}) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)
    if (rc /= {{ variable.name | length }}) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)'
      stop 126
    endif

    rc = f77_zmq_recv(responder, sze, 4, 0)
    if (rc /= 4) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, sze, 4, 0)'
      stop 126
    endif

    if (sze < 0) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, sze, 4, 0) : sze = ', sze
      stop 126
    endif

    rc = f77_zmq_recv(responder, buffer, sze, 0)
    if (rc /= sze) then
      print *, 'ZEZFIO error: ezfio_get_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, buffer, sze, 0)'
      stop 126
    endif

end subroutine ezfio_get_{{category}}_{{variable.name}}

subroutine ezfio_set_{{category}}_{{variable.name}}(buffer)
  use zezfio

    implicit none
    
    {{ typec2stuff[variable.type].fortran }}, intent(inout)  ::  buffer(*)
    integer  ::  rc
    integer  ::  sze 
    integer ::  exit_code

    call ezfio_size_{{category}}_{{variable.name}}(sze)

    rc = f77_zmq_send(responder, "set", 3, ZMQ_SNDMORE)
    if (rc /= 3) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "set", 3, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)
    if (rc /= {{ category | length }}) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ category }}", {{ category | length }}, ZMQ_SNDMORE)'
      stop 126
    endif

    rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, ZMQ_SNDMORE)
    if (rc /= {{ variable.name | length }}) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_send(responder, "{{ variable.name }}", {{ variable.name | length }}, 0)'
      stop 126
    endif

    rc = f77_zmq_send(responder, buffer, sze, 0) 
    if (sze < 0) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, sze, 4, 0) : sze = ', sze
      stop 126
    endif

    rc = f77_zmq_recv(responder, exit_code, 4, 0)
    if (rc /= 4) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  rc = f77_zmq_recv(responder, exit_code, 4, 0)'
      stop 126
    endif

    if (exit_code < 0) then
      print *, 'ZEZFIO error: ezfio_set_{{category}}_{{variable.name}}'
      print *, '  exit_code = ', exit_code
      stop 126
    endif

end subroutine ezfio_set_{{category}}_{{variable.name}}
  {% endfor %}
{% endfor %}

