<?php
    function send_notification ($message)
    {
        include_once 'config.php';
            //URL 뒤의 파라미터를 topic으로 지정
            $url = 'https://fcm.googleapis.com/fcm/send';
            $topic = '/topics/'.$_GET['devcode'];
            $priority_set = 'high';
            $fielde = array('to' => $topic, 'data' => $message, 'priority' => $priority_set);
            $headers = array('Authorization:key ='.GOOGLE_API_KEY, 'Content-Type: application/json');
        //HTTP Request 작성
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_POST, true);
        curl_setopt($ch, CURLOPT_HTTPHEADER, $headers);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
        curl_setopt($ch, CURLOPT_POSTFIELDS, json_encode($fields));
        $result = curl_exec($ch);
        if($result === FALSE){
            die('Curl Failed: '.curl_error($ch));
        }
        curl_close($ch);
        return $result;
    }
    //메시지로 변환 후 전송 시도
    $myMessage = $_POST['message'];
    if($myMessage == ""){
        $myMessage = "주변 ".$topic." 신호등에 녹색불이 켜집니다.";
    }
    $message = array("message" => $myMessage);
    $message_status = send_notification($message);
    //메시지 전송 후 결과 출력
    echo $message_status;
?>