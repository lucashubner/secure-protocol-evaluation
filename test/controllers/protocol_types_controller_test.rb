require 'test_helper'

class ProtocolTypesControllerTest < ActionDispatch::IntegrationTest
  setup do
    @protocol_type = protocol_types(:one)
  end

  test "should get index" do
    get protocol_types_url
    assert_response :success
  end

  test "should get new" do
    get new_protocol_type_url
    assert_response :success
  end

  test "should create protocol_type" do
    assert_difference('ProtocolType.count') do
      post protocol_types_url, params: { protocol_type: { protocolName: @protocol_type.protocolName } }
    end

    assert_redirected_to protocol_type_url(ProtocolType.last)
  end

  test "should show protocol_type" do
    get protocol_type_url(@protocol_type)
    assert_response :success
  end

  test "should get edit" do
    get edit_protocol_type_url(@protocol_type)
    assert_response :success
  end

  test "should update protocol_type" do
    patch protocol_type_url(@protocol_type), params: { protocol_type: { protocolName: @protocol_type.protocolName } }
    assert_redirected_to protocol_type_url(@protocol_type)
  end

  test "should destroy protocol_type" do
    assert_difference('ProtocolType.count', -1) do
      delete protocol_type_url(@protocol_type)
    end

    assert_redirected_to protocol_types_url
  end
end
