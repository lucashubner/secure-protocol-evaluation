require 'test_helper'

class SensorReadsControllerTest < ActionDispatch::IntegrationTest
  setup do
    @sensor_read = sensor_reads(:one)
  end

  test "should get index" do
    get sensor_reads_url
    assert_response :success
  end

  test "should get new" do
    get new_sensor_read_url
    assert_response :success
  end

  test "should create sensor_read" do
    assert_difference('SensorRead.count') do
      post sensor_reads_url, params: { sensor_read: { sensor_type_id: @sensor_read.sensor_type_id, value: @sensor_read.value } }
    end

    assert_redirected_to sensor_read_url(SensorRead.last)
  end

  test "should show sensor_read" do
    get sensor_read_url(@sensor_read)
    assert_response :success
  end

  test "should get edit" do
    get edit_sensor_read_url(@sensor_read)
    assert_response :success
  end

  test "should update sensor_read" do
    patch sensor_read_url(@sensor_read), params: { sensor_read: { sensor_type_id: @sensor_read.sensor_type_id, value: @sensor_read.value } }
    assert_redirected_to sensor_read_url(@sensor_read)
  end

  test "should destroy sensor_read" do
    assert_difference('SensorRead.count', -1) do
      delete sensor_read_url(@sensor_read)
    end

    assert_redirected_to sensor_reads_url
  end
end
